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

ConfigurationCreator& ConfigurationCreator::addModule(const std::string& type, const std::string& name, const ParameterSet& parameters) {
    Configuration::Module module;
    module.name = name;
    module.type = type;
    module.parameters = parameters;

    m_modules.push_back(module);

    return *this;
}

ConfigurationCreator& ConfigurationCreator::setCubaConfiguration(const ParameterSet& parameters) {
    m_cuba_configuration = parameters;

    return *this;
}

ConfigurationCreator& ConfigurationCreator::setGlobalParameters(const ParameterSet& parameters) {
    m_global_parameters = parameters;

    return *this;
}

Configuration ConfigurationCreator::freeze() const {
    Configuration c;

    c.modules = m_modules;
    c.global_parameters = m_global_parameters;
    c.cuba_configuration = m_cuba_configuration;

    c.freeze();

    return c;
}
