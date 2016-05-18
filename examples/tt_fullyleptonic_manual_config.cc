/*
 *  MoMEMta: a modular implementation of the Matrix Element Method
 *  Copyright (C) 2016  Universite catholique de Louvain (UCL), Belgium
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


#include <momemta/ConfigurationCreator.h>
#include <momemta/InputTag.h>
#include <momemta/Logging.h>
#include <momemta/MoMEMta.h>
#include <momemta/ParameterSet.h>
#include <momemta/Utils.h>

#include <TH1D.h>

#include <chrono>

using namespace std::chrono;

int main(int argc, char** argv) {

    UNUSED(argc);
    UNUSED(argv);

    logging::set_level(boost::log::trivial::debug);

    ConfigurationCreator c;

    // Global parameters
    ParameterSet global_parameters;
    global_parameters.set("energy", 13000.).set("top_mass", 173.).set("top_width", 1.491500)
        .set("W_mass", 80.419002).set("W_width", 2.047600);

    c.setGlobalParameters(global_parameters);

    ParameterSet cuba;
    cuba.set("relative_accuracy", 0.01).set("verbosity", (int64_t) 3);

    c.setCubaConfiguration(cuba);

    // BW generator 1 module
    ParameterSet flatter_s13;
    flatter_s13.set("ps_point", InputTag("cuba", "ps_points", 0)).set("mass", global_parameters.get<double>("W_mass")).set("width", global_parameters.get<double>("W_width"));

    c.addModule("BreitWignerGenerator", "flatter_s13", flatter_s13);

//BreitWignerGenerator.flatter_s13 = {
    //-- getpspoint() generates an input tag of type `cuba::ps_points/i`
    //-- where `i` is automatically incremented each time the function is called.
    //ps_point = getpspoint(),
    //mass = parameter('W_mass'),
    //width = parameter('W_width')
//}



    MoMEMta weight(c.freeze());

    // Electron
    LorentzVector p3(16.171895980835, -13.7919054031372, -3.42997527122497, 21.5293197631836);
    // b-quark
    LorentzVector p4(-55.7908325195313, -111.59294128418, -122.144721984863, 174.66259765625);
    // Muon
    LorentzVector p5(-18.9018573760986, 10.0896110534668, -0.602926552295686, 21.4346446990967);
    // Anti b-quark
    LorentzVector p6(71.3899612426758, 96.0094833374023, -77.2513122558594, 142.492813110352);

    auto start_time = system_clock::now();
    std::vector<std::pair<double, double>> weights = weight.computeWeights({p3, p4, p5, p6});
    auto end_time = system_clock::now();

    LOG(debug) << "Result:";
    for (const auto& r: weights) {
        LOG(debug) << r.first << " +- " << r.second;
    }
    
    LOG(info) << "Weight computed in " << std::chrono::duration_cast<milliseconds>(end_time - start_time).count() << "ms";

    return 0;
}
