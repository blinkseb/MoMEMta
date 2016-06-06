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

#include <momemta/Logging.h>
#include <momemta/Module.h>
#include <momemta/ParameterSet.h>
#include <momemta/Types.h>
#include <momemta/Math.h>


#include <TFile.h>
#include <TH2.h>
#include <TAxis.h>

/** \brief Transfer function on energy described by a 2D histogram retrieved from a ROOT file.
 * 
 * This module takes as input a LorentzVector and a phase-space point, generates
 * a new LorentzVector with a different energy (keeping direction and invariant mass),
 * and evaluates the transfer function on the "reconstructed" and "generated" energies.
 *
 * The transfer function (TF) must be a TH2, where the X-axis defines the "generated" (true) energy \f$E_{gen}\f$,
 * and the Y-axis the difference between the reconstructed and the generated energy (\f$E_{rec}-E_{gen}\f$).
 *
 * It is assumed the TF is correctly normalised, i.e. integrals of slices along the Y-axis sum up to 1.
 *
 * The integration is done over the whole width of the TF. The TF is assumed to continue asymptotically as a
 * constant for \f$E_{gen} \to \infty\f$.
 *
 * ### Integration dimension
 *
 * This module adds **1** dimension to the integration.
 *
 * ### Parameters
 *
 *   | Name | Type | %Description |
 *   |------|------|--------------|
 *   | `file` | string | Path to the ROOT file in which the transfer function is saved. |
 *   | `th2_name` | string | Name of the TH2 stored in file `file` |
 *
 * ### Inputs
 *
 *   | Name | Type | %Description |
 *   |------|------|--------------|
 *   | `ps_point` | double | Phase-space point generated by CUBA. |
 *   | `reco_particle` | LorentzVector | Input LorentzVector (experimentally reconstructed particle). |
 *
 * ### Outputs
 *
 *   | Name | Type | %Description |
 *   |------|------|--------------|
 *   | `output` | LorentzVector | Output *generated* LorentzVector, only differing from *reco_particle* by its energy. |
 *   | `TF_times_jacobian` | double | Product of the TF evaluated on the *reco* and *gen* energies, times the jacobian of the transformation needed stretch the integration range from \f$[0,1]\f$ to the width of the TF, times the jacobian \f$dE/d|P|\f$ due to the fact that the integration is done w.r.t \f$|P|\f$, while the TF is parametrised in terms of energy. |
 *
 * \ingroup modules
 */

class BinnedTransferFunctionOnEnergy: public Module {
    public:

        BinnedTransferFunctionOnEnergy(PoolPtr pool, const ParameterSet& parameters): Module(pool, parameters.getModuleName()) {
            m_ps_point = parameters.get<InputTag>("ps_point");
            m_input = parameters.get<InputTag>("reco_particle");

            m_ps_point.resolve(pool);
            m_input.resolve(pool);

            m_file_path = parameters.get<std::string>("file");
            m_th2_name = parameters.get<std::string>("th2_name");

            m_file = std::move( std::unique_ptr<TFile>( TFile::Open(m_file_path.c_str()) ) );
            if(!m_file->IsOpen() || m_file->IsZombie())
                throw file_not_found_error("Could not open file " + m_file_path);

            m_th2 = std::move( std::unique_ptr<TH2>( static_cast<TH2*>( m_file->Get(m_th2_name.c_str()) ) ) );
            if(!m_th2->InheritsFrom("TH2") || !m_th2.get())
                throw th2_not_found_error("Could not retrieve object " + m_th2_name + " deriving from class TH2 in file " + m_file_path + ".");
            m_th2->SetDirectory(0);

            m_deltaMin = m_th2->GetYaxis()->GetXmin();
            m_deltaMax = m_th2->GetYaxis()->GetXmax();
            m_deltaRange = m_deltaMax - m_deltaMin;
            
            TAxis* xAxis = m_th2->GetXaxis();
            m_EgenMin = xAxis->GetXmin();
            m_EgenMax = xAxis->GetXmax();

            // Since we assume the TF continues as a constant for E->infty,
            // we need to be able to retrieve the X axis' last bin, to avoid
            // fetching the TH2's overflow bin.
            m_fallBackEgenMax = xAxis->GetBinCenter(xAxis->GetNbins());
            
            LOG(debug) << "Loaded TH2 " << m_th2_name << " from file " << m_file_path << ".";
            LOG(debug) << "\tDelta range is " << m_deltaMin << " to " << m_deltaMax << ".";
            LOG(debug) << "\tEnergy range is " << m_EgenMin << " to " << m_EgenMax << ".";
            LOG(debug) << "\tWill use values at Egen = " << m_fallBackEgenMax << " for out-of-range values.";
        };

        virtual Status work() override {

            const double& ps_point = m_ps_point.get<double>();
            const LorentzVector& reco_particle = m_input.get<LorentzVector>();

            const double rec_E = reco_particle.E();
            const double range = GetDeltaRange(rec_E);
            const double gen_E = rec_E - GetDeltaMax(rec_E) + range*ps_point;
            const double delta = rec_E - gen_E;

            // To change the particle's energy without changing its direction and mass:
            double gen_pt = std::sqrt(SQ(gen_E) - SQ(reco_particle.M())) / std::cosh(reco_particle.Eta());
            output->SetCoordinates(
                    gen_pt * std::cos(reco_particle.Phi()),
                    gen_pt * std::sin(reco_particle.Phi()),
                    gen_pt * std::sinh(reco_particle.Eta()),
                    gen_E);

            // The bin number is a ROOT "global bin number" using a 1D representation of the TH2
            const int bin = m_th2->FindFixBin(std::min(gen_E, m_fallBackEgenMax), delta);
            // Compute TF*jacobian, where the jacobian includes the transformation of [0,1]->[range_min,range_max] and d|P|/dE
            *TF_times_jacobian = m_th2->GetBinContent(bin) * range * dP_over_dE(*output);

            return Status::OK;
        }

        virtual size_t dimensions() const override {
            return 1;
        }

    private:

        InputTag m_ps_point;
        InputTag m_input;

        std::string m_file_path;
        std::string m_th2_name;

        std::unique_ptr<TFile> m_file;
        std::unique_ptr<TH2> m_th2;

        double m_deltaMin, m_deltaMax, m_deltaRange;
        double m_EgenMin, m_EgenMax;
        double m_fallBackEgenMax;

        std::shared_ptr<LorentzVector> output = produce<LorentzVector>("output");
        std::shared_ptr<double> TF_times_jacobian = produce<double>("TF_times_jacobian");

        // We assume TF=0 for Egen < lower limit of the TH2, and
        //           TF=constant for Egen > upper limit of the TH2
        // In the former case, the integrated range is adapted (shortened) to the range where TF!=0
        // TODO: create picture for documenting the TFs
        inline double GetDeltaRange(const double &Erec) const {
            return GetDeltaMax(Erec) - m_deltaMin;
        }

        inline double GetDeltaMax(const double &Erec) const {
            return std::min(m_deltaMax, Erec - m_EgenMin); 
        }
        
        class file_not_found_error: public std::runtime_error{
            using std::runtime_error::runtime_error;
        };
        
        class th2_not_found_error: public std::runtime_error{
            using std::runtime_error::runtime_error;
        };

};
REGISTER_MODULE(BinnedTransferFunctionOnEnergy);
