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

#include <ModuleUtils.h>

#include <momemta/Logging.h>

#include <ModuleDefUtils.h>

bool momemta::validateModuleParameters(const ParameterSet& parameters, const ModuleList& available_modules) {

    // Get module definition
    auto it = std::find_if(available_modules.begin(), available_modules.end(),
                           [&parameters](const ModuleList::value_type& available_module) {
                               // The *name* of the module inside the registry is what we call the
                               // *type* in userland.
                               return available_module.name == parameters.getModuleType();
                           });

    if (it == available_modules.end())
        throw std::runtime_error("A module was declared with a type unknown to the registry. This is not supposed to "
                                         "be possible");

    const auto& module_def = *it;

    std::vector<std::string> errors;
    std::vector<std::string> warnings;

    // Check that all inputs are defined
    for (const auto& input_def: module_def.inputs) {
        // Ignore optional inputs
        if (input_def.optional)
            continue;

        if (! parameters.exists(input_def.name))
            errors.emplace_back("Input not found: " + input_def.name);
    }

    // And attributes
    // TODO: We can imagine here validating also the type of the attribute
    // TODO: Add support for optional attributes
    for (const auto& attr_def: module_def.attributes) {

        // Ignore global or optional attributes
        if (attr_def.global || attr_def.optional)
            continue;

        if (! parameters.exists(attr_def.name))
            errors.emplace_back("Attribute not found: " + attr_def.name);
    }

    // Check for parameters not found in the module definition
    const auto& parameter_names = parameters.getNames();
    for (const auto& name: parameter_names) {

        // Ignore internal parameters
        if (name.length() > 0 && name[0] == '@')
            continue;

        if (! momemta::inputOrAttrExists(name, module_def))
            warnings.emplace_back("Unexpected parameter: " + name);
    }

    if (! warnings.empty()) {
        // Warnings found during validation
        LOG(warning) << "Warnings found during validation of parameters for module "
                     << parameters.getModuleType() << "::" << parameters.getModuleName();
        for (const auto& warning: warnings)
            LOG(warning) << "    " << warning;
        LOG(warning) << "These parameters will never be used by the module, check your configuration file.";
    }

    if (!errors.empty()) {
        // Validation failed. Print errors.
        LOG(error) << "Validation of parameters for module " << parameters.getModuleType() << "::"
                   << parameters.getModuleName() << " failed: ";
        for (const auto& error: errors)
            LOG(error) << "    " << error;

        LOG(error) << "Check your configuration file.";
    }

    return errors.empty();
}