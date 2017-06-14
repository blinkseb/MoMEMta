/*
 *  MoMEMta: a modular implementation of the Matrix Element Method
 *  Copyright (C) 2017  Universite catholique de Louvain (UCL), Belgium
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <Graph2.h>

#include <ModuleUtils.h>

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/topological_sort.hpp>

namespace graph2 {

// Graph definitions

struct Vertex {
    uint32_t id;
    std::string name; // Unique name of the module
    std::string type; // Module type
    momemta::ModuleList::value_type def;
};

struct Edge {
    std::string description;
    bool virt;
};

typedef boost::adjacency_list<boost::listS, boost::listS, boost::bidirectionalS, Vertex, Edge> Graph;

typedef boost::graph_traits<Graph>::vertex_descriptor vertex_t;
typedef boost::graph_traits<Graph>::edge_descriptor edge_t;
typedef boost::graph_traits<Graph>::out_edge_iterator out_edge_iterator_t;
typedef boost::graph_traits<Graph>::in_edge_iterator in_edge_iterator_t;

class vertex_writer {
public:
    vertex_writer(Graph g) : graph(g) {}

    template <class VertexOrEdge>
    void operator()(std::ostream& out, const VertexOrEdge& v) const {
        std::string shape = "ellipse";
        std::string color = "black";
        std::string style = "solid";

        if (graph[v].def.internal) {
            shape = "rectangle";
            color = "black";
            style = "dashed";
        }

        out << "[shape=\"" << shape << "\",color=\"" << color << "\",style=\"" << style
            << "\",label=\"" << graph[v].name << "\"]";
    }
private:
    Graph graph;
};

class edge_writer {
public:
    edge_writer(Graph g) : graph(g) {}

    template <class VertexOrEdge>
    void operator()(std::ostream& out, const VertexOrEdge& e) const {
        std::string color = "black";
        std::string style = "solid";

        if (graph[e].virt) {
            style = "invis";
        }

        out << "[color=\"" << color << "\",style=\"" << style
            << "\",label=\"" << graph[e].description << "\"]";
    }
private:
    Graph graph;
};

void graphviz_export(const Graph& g, const std::string& filename) {

    std::ofstream f(filename.c_str());

    //auto vertices_name = boost::get(&Vertex::name, g);
    //auto edges_name = boost::get(&Edge::description, g);

    boost::write_graphviz(f, g, vertex_writer(g), edge_writer(g), boost::default_writer(), boost::get(&Vertex::id, g));
}


bool isConnectedToByOut(Graph& g, vertex_t vertex, vertex_t to) {
    out_edge_iterator_t o, o_end;
    std::tie(o, o_end) = boost::out_edges(vertex, g);

    for (; o != o_end; ++o) {
        vertex_t target = boost::target(*o, g);
        if (target == to)
            return true;

        if (isConnectedToByOut(g, target, to))
            return true;
    }

    return false;
}

bool isConnectedToByIn(Graph& g, vertex_t vertex, vertex_t to) {
    in_edge_iterator_t i, i_end;
    std::tie(i, i_end) = boost::in_edges(vertex, g);

    for (; i != i_end; ++i) {
        vertex_t source = boost::source(*i, g);
        if (source == to)
            return true;

        if (isConnectedToByIn(g, source, to))
            return true;
    }

    return false;
}

bool isConnectedTo(Graph& g, vertex_t vertex, vertex_t to) {
    if (isConnectedToByOut(g, vertex, to))
        return true;

    return isConnectedToByIn(g, vertex, to);
}

void sort_modules(const momemta::ModuleList& available_modules,
                  const std::vector<Configuration::Module>& requested_modules,
                  std::vector<Configuration::Module>& modules) {


    Graph g;
    uint32_t id = 0;

    auto get_module_def = [&available_modules](const std::string& module_type) {

        auto it = std::find_if(available_modules.begin(), available_modules.end(),
                               [&module_type](const momemta::ModuleList::value_type& m) {
                                   return m.name == module_type;
                               });

        assert(it != available_modules.end());
        return *it;
    };

    // Keep a map of all the vertices for easy access
    std::unordered_map<std::string, vertex_t> vertices;

    // Create graph vertices. Each vertex is a module requested in the configuration file
    for (const auto& module: requested_modules) {
        vertex_t v = boost::add_vertex(g);

        auto& vertex = g[v];
        vertex.id = id++;
        vertex.name = module.name;
        vertex.type = module.type;

        // Attach module definition to the vertex
        vertex.def = get_module_def(module.type);

        vertices.emplace(module.name, v);
    }

    // Create edges, connecting modules together. An edge link module's outputs to module's inputs
    typename boost::graph_traits<Graph>::vertex_iterator vtx_it, vtx_it_end;
    for (std::tie(vtx_it, vtx_it_end) = boost::vertices(g); vtx_it != vtx_it_end; vtx_it++) {

        const auto& vertex = g[*vtx_it];

        // Connect each output of this module to any module needing it
        for (const auto& output: vertex.def.outputs) {

            // Find any module using this output
            for (const auto& module: requested_modules) {

                // Skip ourselves
                if (module.name == vertex.name)
                    continue;

                // Get definition of this new module
                const auto& input_module_def = get_module_def(module.type);
                for (const auto& input: input_module_def.inputs) {
                    // Grab the InputTag for each input, and see if it points to the vertex
                    momemta::gtl::optional<std::vector<InputTag>> inputTags =
                            momemta::getInputTagsForInput(input, *module.parameters);

                    // If the input is optional, we may not have anything
                    if (! inputTags)
                        continue;

                    for (const auto& inputTag: *inputTags) {
                        if (vertex.name == inputTag.module && output.name == inputTag.parameter) {
                            // We have a match, the InputTag points to the vertex output
                            // Create a new edge in the graph

                            edge_t e;
                            bool inserted;
                            std::tie(e, inserted) = boost::add_edge(*vtx_it, vertices.at(module.name), g);

                            auto& edge = g[e];
                            edge.virt = false;
                            edge.description = inputTag.parameter;
                            if (inputTag.isIndexed()) {
                                edge.description += "[" + std::to_string(inputTag.index) + "]";
                            }
                        }
                    }
                }
            }
        }
    }

    // Modules are connected together. Find all vertices not connected to anything and remove them
    for (auto it = vertices.begin(), ite = vertices.end(); it != ite;) {
        if (boost::out_degree(it->second, g) == 0) {

            auto& vertex = g[it->second];

            // Don't consider internal modules
            if (vertex.def.internal) {
                ++it;
                continue;
            }

            // Check in the definition if this module has any output
            // If not, we keep it.
            if (vertex.def.outputs.empty()) {
                ++it;
                continue;
            }

            // Otherwise, remove it
            LOG(info) << "Module '" << it->first << "' output is not used by any other module. Removing it from the configuration.";
            boost::clear_vertex(it->second, g);
            boost::remove_vertex(it->second, g);

            it = vertices.erase(it);
        } else
            ++it;
    }

    // We need to make sure that any dependencies of a module inside a looper
    // is ran before the looper itself.
    for (const auto& vertex: vertices) {
        if (g[vertex.second].type == "Looper") {

            out_edge_iterator_t e, e_end;
            std::tie(e, e_end) = boost::out_edges(vertex.second, g);

            // Iterator over all edges this Looper vertex is connected to
            for (; e != e_end; ++e) {
                auto target = boost::target(*e, g);

                // Iterate over all edges connected to the module
                in_edge_iterator_t i, i_end;
                std::tie(i, i_end) = boost::in_edges(target, g);

                for (; i != i_end; ++i) {
                    auto source = boost::source(*i, g);

                    if (source == vertex.second)
                        continue;

                    // Check if the source vertex is connected to the looper in any way
                    if (!isConnectedTo(g, source, vertex.second)) {
                        edge_t e;
                        bool inserted;
                        std::tie(e, inserted) = boost::add_edge(source, vertex.second, g);
                        g[e].description = "virtual link";
                        g[e].virt = true;
                    }
                }
            }
        }
    }

    // Ensure ids are continuous
    id = 0;
    for (std::tie(vtx_it, vtx_it_end) = boost::vertices(g); vtx_it != vtx_it_end; vtx_it++) {
        g[*vtx_it].id = id++;
    }

    // Sort graph
    std::list<vertex_t> sorted_vertices;
    try {
        boost::topological_sort(g, std::front_inserter(sorted_vertices),
                                boost::vertex_index_map(boost::get(&Vertex::id, g)));
    } catch (...) {
        graphviz_export(g, "graph.debug");
        LOG(fatal) << "Exception while sorting the graph. Graphviz representation saved as graph.debug";
        throw;
    }

    // Remove vertices corresponding to internal modules
    sorted_vertices.erase(std::remove_if(sorted_vertices.begin(), sorted_vertices.end(),
                                         [&g](const vertex_t& vertex) -> bool {
                                             return g[vertex].def.internal;
                                         }), sorted_vertices.end());

    for (const auto vertex: sorted_vertices) {
        LOG(info) << "[Graph2] Running " << g[vertex].name;
    }

    graphviz_export(g, "graph2.dot");

    for (auto vertex: sorted_vertices) {
        auto it = std::find_if(requested_modules.begin(), requested_modules.end(),
                               [&vertex, &g](const Configuration::Module& module) -> bool {
                                   return module.name == g[vertex].name;
                               });

        assert(it != requested_modules.end());
        modules.push_back(*it);
    }
}

}