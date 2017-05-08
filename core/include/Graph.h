#pragma once

#include <momemta/Configuration.h>
#include <momemta/Module.h>

#include <ExecutionPath.h>

#include <map>
#include <string>
#include <vector>

/// Generic graph representation of the module hierarchy
namespace graph2 {

/**
 * \brief Sort the list of modules declared in the configuration according to their dependencies.
 *
 * Modules are sorted based on their inputs and outputs dependencies, ensuring modules producing quantities
 * are executed before modules depending on these quantities. If a module does not contribute, it's ignored
 * and won't be present in the sorted list.
 *
 * If sorting is not possible (cyclic dependencies for example), an exception is thrown
 *
 * \param[in] available_modules List of all available modules, with their definition.
 * \param[in] requested_modules List of modules to sort, as declared in the configuration file.
 * \param[in] execution_paths List of execution paths declared in the configuration file
 * \param[out] modules Sorted list of modules, mapped to execution paths.
 *
 * \sa momemta::ModuleRegistry::exportList()
 */

// We can't use a map here as order of execution path is important
typedef std::vector<std::pair<boost::uuids::uuid, std::vector<Configuration::ModuleDecl>>> SortedModuleList;
void sort_modules(
        const momemta::ModuleList& available_modules,
        const std::vector<Configuration::ModuleDecl>& requested_modules,
        const std::vector<ExecutionPath*>& execution_paths,
        SortedModuleList& modules
);

/**
 * \brief Export a given graph in `dot` format
 *
 * \note To produce a PDF, run the following command
 * ```
 * dot -Tpdf filename.dot -o filename.pdf
 * ```
 *
 * \param g The graph to export
 * \param filename The output filename
 */
//void graphviz_export(const Graph& g, const std::string& filename);

}
