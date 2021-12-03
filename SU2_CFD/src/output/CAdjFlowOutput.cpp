/*!
 * \file CAdjFlowOutput.cpp
 * \brief Main subroutines for flow discrete adjoint output
 * \author T. Kattmann
 * \version 7.2.1 "Blackbird"
 *
 * SU2 Project Website: https://su2code.github.io
 *
 * The SU2 Project is maintained by the SU2 Foundation
 * (http://su2foundation.org)
 *
 * Copyright 2012-2021, SU2 Contributors (cf. AUTHORS.md)
 *
 * SU2 is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * SU2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with SU2. If not, see <http://www.gnu.org/licenses/>.
 */

#include "../../include/output/CAdjFlowOutput.hpp"

#include "../../include/solvers/CSolver.hpp"

CAdjFlowOutput::CAdjFlowOutput(CConfig* config, unsigned short nDim) : COutput(config, nDim, false) {
  turb_model = config->GetKind_Turb_Model();
  cont_adj = config->GetContinuous_Adjoint();
  frozen_visc = (config->GetFrozen_Visc_Disc() && !cont_adj) || (config->GetFrozen_Visc_Cont() && cont_adj);
}

void CAdjFlowOutput::AddHistoryOutputFields_AdjScalarRMS_RES(const CConfig* config) {
  if (!frozen_visc) {
    switch (turb_model) {
      case TURB_MODEL::SA:
      case TURB_MODEL::SA_NEG:
      case TURB_MODEL::SA_E:
      case TURB_MODEL::SA_COMP:
      case TURB_MODEL::SA_E_COMP:
        /// DESCRIPTION: Root-mean square residual of the adjoint nu tilde.
        AddHistoryOutput("RMS_ADJ_NU_TILDE", "rms[A_nu]", ScreenOutputFormat::FIXED, "RMS_RES",
                         "Root-mean square residual of the adjoint nu tilde.", HistoryFieldType::RESIDUAL);
        break;
      case TURB_MODEL::SST:
      case TURB_MODEL::SST_SUST:
        /// DESCRIPTION: Root-mean square residual of the adjoint kinetic energy.
        AddHistoryOutput("RMS_ADJ_TKE", "rms[A_k]", ScreenOutputFormat::FIXED, "RMS_RES",
                         "Root-mean square residual of the adjoint kinetic energy.", HistoryFieldType::RESIDUAL);
        /// DESCRIPTION: Root-mean square residual of the adjoint dissipation.
        AddHistoryOutput("RMS_ADJ_DISSIPATION", "rms[A_w]", ScreenOutputFormat::FIXED, "RMS_RES",
                         " Root-mean square residual of the adjoint dissipation.", HistoryFieldType::RESIDUAL);
        break;
      case TURB_MODEL::NONE:
        break;
    }
  }

  if (config->GetKind_Species_Model() != SPECIES_MODEL::NONE) {
    for (unsigned short iVar = 0; iVar < config->GetnSpecies(); iVar++) {
      AddHistoryOutput("RMS_ADJ_SPECIES_" + std::to_string(iVar), "rms[A_rho*Y_" + std::to_string(iVar) + "]",
                       ScreenOutputFormat::FIXED, "RMS_RES",
                       "Root-mean square residual of the adjoint transported species.", HistoryFieldType::RESIDUAL);
    }
  }
}

void CAdjFlowOutput::AddHistoryOutputFields_AdjScalarMAX_RES(const CConfig* config) {
  if (!frozen_visc) {
    switch (turb_model) {
      case TURB_MODEL::SA:
      case TURB_MODEL::SA_NEG:
      case TURB_MODEL::SA_E:
      case TURB_MODEL::SA_COMP:
      case TURB_MODEL::SA_E_COMP:
        /// DESCRIPTION: Maximum residual of the adjoint nu tilde.
        AddHistoryOutput("MAX_ADJ_NU_TILDE", "max[A_nu]", ScreenOutputFormat::FIXED, "MAX_RES",
                         "Maximum residual of the adjoint nu tilde.", HistoryFieldType::RESIDUAL);
        break;
      case TURB_MODEL::SST:
      case TURB_MODEL::SST_SUST:
        /// DESCRIPTION: Maximum residual of the adjoint kinetic energy.
        AddHistoryOutput("MAX_ADJ_TKE", "max[A_k]", ScreenOutputFormat::FIXED, "MAX_RES",
                         "Maximum residual of the adjoint kinetic energy.", HistoryFieldType::RESIDUAL);
        /// DESCRIPTION: Maximum residual of the adjoint dissipation.
        AddHistoryOutput("MAX_ADJ_DISSIPATION", "max[A_w]", ScreenOutputFormat::FIXED, "MAX_RES",
                         "Maximum residual of the adjoint dissipation.", HistoryFieldType::RESIDUAL);
        break;
      case TURB_MODEL::NONE:
        break;
    }
  }

  if (config->GetKind_Species_Model() != SPECIES_MODEL::NONE) {
    for (unsigned short iVar = 0; iVar < config->GetnSpecies(); iVar++) {
      AddHistoryOutput("MAX_ADJ_SPECIES_" + std::to_string(iVar), "max[A_rho*Y_" + std::to_string(iVar) + "]",
                       ScreenOutputFormat::FIXED, "RMS_RES", "Maximum residual of the adjoint transported species.",
                       HistoryFieldType::RESIDUAL);
    }
  }
}

void CAdjFlowOutput::AddHistoryOutputFields_AdjScalarBGS_RES(const CConfig* config) {
  if (!frozen_visc) {
    switch (turb_model) {
      case TURB_MODEL::SA:
      case TURB_MODEL::SA_NEG:
      case TURB_MODEL::SA_E:
      case TURB_MODEL::SA_COMP:
      case TURB_MODEL::SA_E_COMP:
        /// DESCRIPTION: BGS residual of the adjoint nu tilde.
        AddHistoryOutput("BGS_ADJ_NU_TILDE", "bgs[A_nu]", ScreenOutputFormat::FIXED, "BGS_RES",
                         "BGS residual of the adjoint nu tilde.", HistoryFieldType::RESIDUAL);
        break;
      case TURB_MODEL::SST:
      case TURB_MODEL::SST_SUST:
        /// DESCRIPTION: BGS residual of the adjoint kinetic energy.
        AddHistoryOutput("BGS_ADJ_TKE", "bgs[A_k]", ScreenOutputFormat::FIXED, "BGS_RES",
                         "BGS residual of the adjoint kinetic energy.", HistoryFieldType::RESIDUAL);
        /// DESCRIPTION: BGS residual of the adjoint dissipation.
        AddHistoryOutput("BGS_ADJ_DISSIPATION", "bgs[A_w]", ScreenOutputFormat::FIXED, "BGS_RES",
                         "BGS residual of the adjoint dissipation.", HistoryFieldType::RESIDUAL);
        break;
      case TURB_MODEL::NONE:
        break;
    }
  }

  if (config->GetKind_Species_Model() != SPECIES_MODEL::NONE) {
    for (unsigned short iVar = 0; iVar < config->GetnSpecies(); iVar++) {
      AddHistoryOutput("BGS_ADJ_SPECIES_" + std::to_string(iVar), "bgs[A_rho*Y_" + std::to_string(iVar) + "]",
                       ScreenOutputFormat::FIXED, "RMS_RES", "BGS residual of the adjoint transported species.",
                       HistoryFieldType::RESIDUAL);
    }
  }
}

void CAdjFlowOutput::AddHistoryOutputFields_AdjScalarLinsol(const CConfig* config) {
  if ((turb_model != TURB_MODEL::NONE) && !frozen_visc) {
    AddHistoryOutput("LINSOL_ITER_TURB", "LinSolIterTurb", ScreenOutputFormat::INTEGER, "LINSOL",
                     "Number of iterations of the linear solver for turbulence.");
    AddHistoryOutput("LINSOL_RESIDUAL_TURB", "LinSolResTurb", ScreenOutputFormat::FIXED, "LINSOL",
                     "Residual of the linear solver for turbulence.");
  }

  if (config->GetKind_Species_Model() != SPECIES_MODEL::NONE) {
    AddHistoryOutput("LINSOL_ITER_SPECIES", "LinSolIterSpecies", ScreenOutputFormat::INTEGER, "LINSOL",
                     "Number of iterations of the linear solver for species solver.");
    AddHistoryOutput("LINSOL_RESIDUAL_SPECIES", "LinSolResSpecies", ScreenOutputFormat::FIXED, "LINSOL",
                     "Residual of the linear solver for species solver.");
  }
}

void CAdjFlowOutput::LoadHistoryData_AdjScalar(const CConfig* config, const CSolver* const* solver) {
  const auto adjturb_solver = solver[ADJTURB_SOL];
  const auto adjspecies_solver = solver[ADJSPECIES_SOL];

  if (!frozen_visc) {
    switch (turb_model) {
      case TURB_MODEL::SA:
      case TURB_MODEL::SA_NEG:
      case TURB_MODEL::SA_E:
      case TURB_MODEL::SA_COMP:
      case TURB_MODEL::SA_E_COMP:
        SetHistoryOutputValue("RMS_ADJ_NU_TILDE", log10(adjturb_solver->GetRes_RMS(0)));
        SetHistoryOutputValue("MAX_ADJ_NU_TILDE", log10(adjturb_solver->GetRes_Max(0)));
        SetHistoryOutputValue("BGS_ADJ_NU_TILDE", log10(adjturb_solver->GetRes_BGS(0)));
        break;
      case TURB_MODEL::SST:
      case TURB_MODEL::SST_SUST:
        SetHistoryOutputValue("RMS_ADJ_TKE", log10(adjturb_solver->GetRes_RMS(0)));
        SetHistoryOutputValue("RMS_ADJ_DISSIPATION", log10(adjturb_solver->GetRes_RMS(1)));
        SetHistoryOutputValue("MAX_ADJ_TKE", log10(adjturb_solver->GetRes_Max(0)));
        SetHistoryOutputValue("MAX_ADJ_DISSIPATION", log10(adjturb_solver->GetRes_Max(1)));
        SetHistoryOutputValue("BGS_ADJ_TKE", log10(adjturb_solver->GetRes_BGS(0)));
        SetHistoryOutputValue("BGS_ADJ_DISSIPATION", log10(adjturb_solver->GetRes_BGS(1)));
        break;
      case TURB_MODEL::NONE:
        break;
    }

    if (turb_model != TURB_MODEL::NONE) {
      SetHistoryOutputValue("LINSOL_ITER_TURB", adjturb_solver->GetIterLinSolver());
      SetHistoryOutputValue("LINSOL_RESIDUAL_TURB", log10(adjturb_solver->GetResLinSolver()));
    }
  }

  if (config->GetKind_Species_Model() != SPECIES_MODEL::NONE) {
    for (unsigned short iVar = 0; iVar < config->GetnSpecies(); iVar++) {
      SetHistoryOutputValue("RMS_ADJ_SPECIES_" + std::to_string(iVar), log10(adjspecies_solver->GetRes_RMS(iVar)));
      SetHistoryOutputValue("MAX_ADJ_SPECIES_" + std::to_string(iVar), log10(adjspecies_solver->GetRes_Max(iVar)));
      SetHistoryOutputValue("BGS_ADJ_SPECIES_" + std::to_string(iVar), log10(adjspecies_solver->GetRes_BGS(iVar)));
    }

    SetHistoryOutputValue("LINSOL_ITER_SPECIES", adjspecies_solver->GetIterLinSolver());
    SetHistoryOutputValue("LINSOL_RESIDUAL_SPECIES", log10(adjspecies_solver->GetResLinSolver()));
  }
}

void CAdjFlowOutput::SetVolumeOutputFields_AdjScalarSolution(const CConfig* config) {}

void CAdjFlowOutput::SetVolumeOutputFields_AdjScalarResidual(const CConfig* config) {}

void CAdjFlowOutput::LoadVolumeData_AdjScalar(const CConfig* config, const CSolver* const* solver,
                                              const CGeometry* geometry, const unsigned long iPoint) {}
