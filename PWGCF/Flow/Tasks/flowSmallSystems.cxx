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
  O2_DEFINE_CONFIGURABLE(cfgPtMin, float, 0.2, "Minimum pt (GeV/c)");
  O2_DEFINE_CONFIGURABLE(cfgPtMax, float, 10, "Maximum pt (GeV/c)");
  O2_DEFINE_CONFIGURABLE(cfgEta, float, 0.8, "Eta cut");
  O2_DEFINE_CONFIGURABLE(cfgVtxZ, float, 10, "Vertex cut in z (cm)");
  HistogramRegistry registry{"HistRegistry"};

  // Equivalent of the AliRoot task UserCreateOutputObjects
  void init(InitContext const&)
  {
    // Define your axes
    // Constant bin width axis
    std::vector<double> cfgPtBinning = {0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95,
                                          1, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9,
                                          2, 2.2, 2.4, 2.6, 2.8,
                                          3, 3.5, 4, 5, 6,
                                          8, 10};
    AxisSpec axisPt = {cfgPtBinning, "#it{p}_{T} (GeV/#it{c})"};
    // Add histograms to histogram manager (as in the output object of in AliPhysics)
    registry.add("hPt", ";#it{p}_{T} (GeV/#it{c})", HistType::kTH1D, {axisPt});
    registry.add("hVtxZ",";z (cm)", HistType::kTH1D, {{120, -20, 20}});
  }
  Filter collisionFilter = nabs(aod::collision::posZ) < cfgVtxZ;
  Filter trackFilter = nabs(aod::track::eta) < cfgEta && (aod::track::pt > cfgPtMin && aod::track::pt < cfgPtMax);
  // Equivalent of the AliRoot task UserExec
  using MyCollision = soa::Filtered<aod::Collisions>;
  using MyTrack = soa::Filtered<aod::Tracks>;

  void process(MyCollision::iterator const& collision, MyTrack const& Tracks)
  {
    // Performing the event selection
    registry.fill(HIST("hVtxZ"), collision.posZ());
    for (const auto& track : Tracks) { // Loop over tracks
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