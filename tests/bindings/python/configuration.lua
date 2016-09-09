function append(t1, t2)
    for i = 1, #t2 do
        t1[#t1 + 1] = t2[i]
    end

    return t1
end

inputs = {
    'tf_p1::output',
    'permutator::output/1',
    'tf_p3::output',
    'permutator::output/2',
}

parameters = {
    energy = 13000.,
    top_mass = 173.,
    top_width = 1.491500,
    W_mass = 80.419002,
    W_width = 2.047600,
}

cuba = {
    relative_accuracy = 0.01,
    verbosity = 0,
}

BreitWignerGenerator.flatter_s13 = {
    ps_point = add_dimension(),
    mass = parameter('W_mass'),
    width = parameter('W_width')
}

BreitWignerGenerator.flatter_s134 = {
    ps_point = add_dimension(),
    mass = parameter('top_mass'),
    width = parameter('top_width')
}

BreitWignerGenerator.flatter_s25 = {
    ps_point = add_dimension(),
    mass = parameter('W_mass'),
    width = parameter('W_width')
}

BreitWignerGenerator.flatter_s256 = {
    ps_point = add_dimension(),
    mass = parameter('top_mass'),
    width = parameter('top_width')
}

GaussianTransferFunction.tf_p1 = {
    ps_point = add_dimension(),
    reco_particle = 'input::particles/1',
    sigma = 0.05,
}

GaussianTransferFunction.tf_p2 = {
    ps_point = add_dimension(),
    reco_particle = 'input::particles/2',
    sigma = 0.10,
}

GaussianTransferFunction.tf_p3 = {
    ps_point = add_dimension(),
    reco_particle = 'input::particles/3',
    sigma = 0.05,
}

GaussianTransferFunction.tf_p4 = {
    ps_point = add_dimension(),
    reco_particle = 'input::particles/4',
    sigma = 0.10,
}

-- Declare module before the permutator to test read-access in the pool
-- for non-existant values.
BlockD.blockd = {
    inputs = inputs,
    pT_is_met = true,
    s13 = 'flatter_s13::s',
    s134 = 'flatter_s134::s',
    s25 = 'flatter_s25::s',
    s256 = 'flatter_s256::s',
}

Permutator.permutator = {
    ps_point = add_dimension(),
    inputs = {
        'tf_p2::output',
        'tf_p4::output',
    }
}

-- Loop over block solutions

Looper.looper = {
    solutions = "blockd::solutions",
    path = Path("boost", "ttbar", "dmem", "integrand")
}

BuildInitialState.boost = {
    solution = 'looper::solution',
    do_transverse_boost = true,
    particles = inputs
}

jacobians = { 'flatter_s13::jacobian', 'flatter_s134::jacobian', 'flatter_s25::jacobian', 'flatter_s256::jacobian' }
append(jacobians, { 'tf_p1::TF_times_jacobian', 'tf_p2::TF_times_jacobian', 'tf_p3::TF_times_jacobian', 'tf_p4::TF_times_jacobian' })


MatrixElement.ttbar = {
    pdf = 'CT10nlo',
    pdf_scale = parameter('top_mass'),
    matrix_element = 'pp_ttx_fully_leptonic',
    matrix_element_parameters = {
        card = '../MatrixElements/Cards/param_card.dat'
    },
    initialState = 'boost::partons',
    invisibles = {
        input = 'looper::solution',
        ids = {
            {
                pdg_id = 12,
                me_index = 2,
            },

            {
                pdg_id = -14,
                me_index = 5,
            }
        }
    },
    particles = {
        inputs = inputs,
        ids = {
            {
                pdg_id = -11,
                me_index = 1,
            },

            {
                pdg_id = 5,
                me_index = 3,
            },

            {
                pdg_id = 13,
                me_index = 4,
            },

            {
                pdg_id = -5,
                me_index = 6,
            },
        }
    },
    jacobians = jacobians
}

DoubleSummer.integrand = {
    input = "ttbar::output"
}

-- End of loop
integrand("integrand::sum")
