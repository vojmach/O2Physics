// Copyright 2019-2020 CERN and copyright holders of ALICE O2.
// See https://alice-o2.web.cern.ch/copyright for details of the copyright holders.
// All rights not expressly granted are reserved.
//
// This software is distributed under the terms of the GNU General Public
// License v3 (GPL Version 3), copied verbatim in the file "COPYING".
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

/// \file   flowSmallSystems.cxx
/// \author Vojtech Machacek (vojtech.machacek@cern.ch)
/// \since  Jun 17, 2024
/// \brief  Multiparticle flow measurements for small systems

//#include "Common/DataModel/EventSelection.h"
//#include "Common/Core/TrackSelection.h"
//#include "Common/DataModel/TrackSelectionTables.h"
#include <vector>
#include "Framework/runDataProcessing.h"
#include "Framework/AnalysisTask.h"
//#include "Framework/ASoAHelpers.h"
//#include "Framework/RunningWorkflowInfo.h"
//#include "Framework/HistogramRegistry.h"

#define O2_DEFINE_CONFIGURABLE(NAME, TYPE, DEFAULT, HELP) Configurable<TYPE> NAME{#NAME, DEFAULT, HELP};

using namespace o2;
using namespace o2::framework;
using namespace o2::framework::expressions;

// STEP 0
// Example task illustrating how to create histograms and fill them with basic information.
// A basic event selection is applied.

struct FlowSmallSystems {
  O2_DEFINE_CONFIGURABLE(cfgPtMin, double, 0.2, "Minimum pt (GeV/c)");
  O2_DEFINE_CONFIGURABLE(cfgPtMax, double, 10.0, "Maximum pt (GeV/c)");
  HistogramRegistry registry{"HistRegistry"};

  // Equivalent of the AliRoot task UserCreateOutputObjects
  void init(InitContext&)
  {
    // Define your axes
    // Constant bin width axis
    std::vector<double> ptBinning = {cfgPtMin};
    while (ptBinning.back() < cfgPtMax)
    {
      if (ptBinning.back() < 1.0) {
        ptBinning.emplace_back(ptBinning.back() + 0.1);
      } else if (ptBinning.back() < 5.0) {
        ptBinning.emplace_back(ptBinning.back() + 0.5);
      } else if (ptBinning.back() < 10.0) {
        ptBinning.emplace_back(ptBinning.back() + 1.0);
      } else {
        ptBinning.emplace_back(ptBinning.back() + 10.0);
      }
    }
    AxisSpec PtAxis = {ptBinning, "#it{p}_{T} (GeV/#it{c})"};
    // Add histograms to histogram manager (as in the output object of in AliPhysics)
    registry.add("hPt", ";#it{p}_{T} (GeV/#it{c})", HistType::kTH1D, {PtAxis});
    registry.add("hVtxZ",";z (cm)", HistType::kTH1D, {{160, -20, 20}});
  }

  // Equivalent of the AliRoot task UserExec
  void process(aod::Collision const& coll, aod::Tracks const& Tracks)
  {
    // Performing the event selection
    registry.fill(HIST("hVtxZ"), coll.posZ());

    for (auto track : Tracks) { // Loop over tracks
      registry.fill(HIST("hPt"), track.pt());
    }
  }
};


WorkflowSpec defineDataProcessing(ConfigContext const& cfgc)
{
  return WorkflowSpec{
    adaptAnalysisTask<FlowSmallSystems>(cfgc),
  };
}