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

#pragma once

#include <momemta/ModuleRegistry.h>
#include <momemta/ParameterSet.h>

#include <lib/optional.h>

namespace momemta {

bool validateModuleParameters(const ParameterSet& parameters, const ModuleList& available_modules);

/**
 * Return the list of InputTag associated with a given input, if it exists
 *
 * \param input The input definition
 * \param parameters Modules' parameters where to look for the InputTag
 * \return The list of InputTag associated with \p input, if found in \p parameters.
 */
gtl::optional<std::vector<InputTag>> getInputTagsForInput(const ArgDef& input, const ParameterSet& parameters);

}