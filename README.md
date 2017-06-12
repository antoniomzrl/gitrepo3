
LEPTS project / Low-Energy Particle Track Simulation
----------------------------------------------------

Monte Carlo-based simulations of radiation transport through biological
tissues constitute an important complement to experimental dosimetry for
the assessment of radiation damage in radioprotection as well as clinical
applications such as diagnostics and radiotherapy. In comparison with
conventional dose calculation methods that combine empirical data and
deterministic algorithms, they offer significant improvements
particularly in conditions involving inhomogeneous materials or
geometrically complex irradiation conditions.

  Therefore, various Monte Carlo (MC) codes oriented towards
radiotherapeutic and medical imaging applications have been developed in
the last decades. Those programmes provide particle tracking making use of
public reference databases and include sophisticated elements that help
reduce calculation time, e.g. condensed-history algorithms. Additionally,
electrons are usually forced to instantly deposit all of their remaining
energy below a certain cut-off value, further speeding up simulations.

  Generally, energy deposition in a given volume is assumed to be directly
proportional to the number of ionization events that have been produced
therein. However, during the irradiation of biological tissues and almost
irrespective of the incident radiation quality, a considerable portion of
energy dose is eventually deposited in the target material by secondary
electrons through multiple collisions. Only recent discoveries have shown
that molecular damage (e.g., molecular dissociations or strand breaks in
DNA) can be induced in biomolecules very efficiently even by sub-ionising
electrons through molecular resonances and dissociative electron
attachment.

  In view of this, an interaction model capable of giving a realistic,
physically meaningful description of the effective genotoxic damage caused
by the incident radiation in a biological tissue should improve on
existing MC codes in the following aspects:

  –The simulation has to take into account the molecular nature of the
   absorber medium in order to predict the physical or chemical
   alterations actually induced in its components. On one hand, this means
   that input data for a given material can no longer be computed as the
   sum of its atomic constituents, but needs to be supplied specifically
   for each molecule. On the other hand, it implies that each collision
   has to be simulated explicitly (event by event), without using
   approximations treating multiple scattering events as a single process.
   This approach permits to obtain particle tracks with real nanometric
   detail.

  –All different kinds of known inelastic collisions have to be
   considered in the interaction model instead of restricting inelastic
   events exclusively to ionizations. Only by including those inelastic
   channels, a complete picture of the effects induced in the irradiated
   medium can be obtained. In particular, all relevant interaction
   mechanisms leading directly or indirectly to molecular dissociations
   need to be taken into account. These include, amongst others, neutral
   dissociation and dissociative electron attachment for causing chemical
   alterations and radical formation (which, for the particular example
   of biological materials, can ultimately lead to single or double
   breaks in RNA or DNA strands and protein malfunctions).

  –Finally, low-energy electrons cannot be ignored by the interaction
   model by applying cut-off values. Electrons should be tracked until
   thermalization in order to include scattering events that occur only
   at low energies, even below ionization threshold. By including this
   amendment, also the circumstance that collectively, low-energy
   secondary electrons can carry away a considerable amount of energy
   from the primary particle’s path and produce interactions in the
   surrounding tissue is accounted for. Consequently, interaction data
   has to be collected for this energy range, as well. The global aim
   of a simulation fulfilling these requirements would be to predict
   radiation damage in biological tissues at the molecular level,
   ultimately by calculating how exactly specific proteins, DNA
   strands, or other functional elements are affected by irradiation in
   particular conditions (nanodosimetric approach). At present, this
   objective is not resolved due to the still scarce results on
   radiation-matter interactions for biomolecules. However, here we
   present the code Low-Energy Particle Track Simulation (LEPTS) which
   has been specifically designed by us as a tool for nanodosimetry
   that offers the improvements exposed above. It distinguishes fine
   details in the electron interaction model and gives a molecular-level
   description of the processes involved in radiation transport and
   energy degradation down to about 1 eV. Furthermore, it is a flexible
   programme prepared to include the results of new investigations as
   they become available through constant revision and maintenance of
   the scattering subroutines and the underlying interaction data sets.
   Apart from electron transport in irradiated materials of biomedical
   interest, also positron interactions (particularly interesting for
   imaging applications such as PET, positron emission tomography) can
   be simulated with LEPTS..
_
_