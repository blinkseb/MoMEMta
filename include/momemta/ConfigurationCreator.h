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


#pragma once

#include <memory>
#include <string>
#include <vector>

#include <momemta/Configuration.h>

class ParameterSet;

class ConfigurationCreator {
    public:
        ConfigurationCreator() = default;

        ConfigurationCreator& addModule(const std::string&, const std::string&, const ParameterSet&);
        ConfigurationCreator& setCubaConfiguration(const ParameterSet&);
        ConfigurationCreator& setGlobalParameters(const ParameterSet&);

        Configuration freeze() const;

    private:
        std::vector<Configuration::Module> m_modules;
        ParameterSet m_cuba_configuration;
        ParameterSet m_global_parameters;
};
