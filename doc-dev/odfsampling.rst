.. _odfsampling:

Random sampling of ODFs
=======================

Given an ODF defined on a mesh, either on elements (piecewise constant) or on nodes (piecewise linear), how can we sample it randomly?

Case of a uniform ODF
---------------------

If the ODF is uniform (no texture), then we can choose the orientations without taking it into account, from random numbers (:math:`n_1`, :math:`n_2`, :math:`n_3`, :math:`n_4 \in [0,\,1]`):

- Euler-Bunge angles: :math:`\varphi_1=2\,\pi\,n_1`, :math:`\varphi_2= \hbox{acos} (2\,n_2-1)`, :math:`\varphi_3=2\,\pi\,n_3`
- unit quaternion: if :math:`{{n_1}^2+{n_2}^2+{n_3}^2+{n_4}^2} \leq 1` then accept orientation :math:`q_i=n_i`
- homochoric vector:  if :math:`\sqrt{{n_1}^2+{n_2}^2+{n_3}^2} \leq 1` then accept orientation :math:`x_i=n_i`

Case of a non-uniform ODF
-------------------------

.. note::

  Reminder on the expressions of the volume element, :math:`dg`:

  1. Rodrigues space:

    - :math:`dg = \left(\frac{\rho}{1+\rho^2}\right)^2 \, d\rho \sin{\chi} \, d\chi \, d\zeta` (polar coordinates)
    - :math:`dg = \left(\frac{1}{1+\rho^2}\right)^2 \, dr_1 \, dr_2 \, dr_3` (rectangular coordinates)

    - By definition, :math:`\int_g dg = \pi^2` (with :math:`\rho\in\left[0,\,+\infty\right[`, :math:`\chi\in\left[0,\,\pi\right]` and :math:`\zeta\in\left[0,\,2\,\pi\right]` or :math:`\left\{r_1,\, r_2,\, r_3\right\} \in \mathbb{R}^3`).  If integration is done on a fundamental region, then the integral is equal to :math:`\pi^2/n_c`, where :math:`n_c` is the multiplicity (24 for cubic, etc.).

  2. Euler space (Bunge convention):

    - :math:`dg = \sin{\phi} \, d\varphi_1 \, d\phi \, d\varphi_2`

    - By definition, :math:`\int_{\varphi_1=0}^{2\,\pi}  \int_{\phi=0}^{\pi/2} \int_{\varphi_2=0}^{\pi/2} dg = \pi^2`

    - :math:`\sqrt{\hbox{det}(g)} = \frac{1}{8\,\pi^2} \sin{\phi}`, where :math:`g` is the metric tensor

  3. Homochoric space:

 The integral of the ODF, :math:`f(g)`, over full orientation space is therefore equal to :math:`\pi^2`.  The integral over the cubic fundamental region is equal to :math:`\pi^2/24`.

If the ODF is non-uniform, the frequency of orientations must adhere to the intensity of the ODF (by definition).  One may distinguish random sampling and uniform sampling, however.  Uniform sampling (just as we implemented it for the case of a uniform ODF) attempts to fit the ODF at best given the number of orientations available.  Random sampling (just as we apply it to the case of a uniform ODF) follows the ODF from a statistical point of view, but does not attempt to fit it at best; it retains a random character.  This is the case developed here.

Let the ODF be defined as

.. math::

  \begin{equation}
    f(\boldsymbol{x}) = \frac{1}{V} \, \frac{dV(\boldsymbol{x})}{d\boldsymbol{x}},
  \end{equation}

where :math:`\boldsymbol{x}` is the orientation.

Rejection sampling (inefficient)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Let :math:`f_\text{max}` be the maximum of :math:`f`.  The procedure is then the following:

  1. Pick a random orientation, :math:`\boldsymbol{x}`, following a uniform ODF;
  2. Generate a random number :math:`t\in[0,\,1]`;
  3. if :math:`t < f(\boldsymbol{x}) / f_\text{max}`, then accept orientation.

Terminate when all orientations have been generated.

The efficiency of the method scales with :math:`1/f_\text{max}`; that is, the stronger the texture is, the more orientations need to be generated and tested to attain the desired number of orientations.  One may also note that the evaluation of :math:`f` at a particular orientation may be costly (for large meshes), which makes the process relatively inefficient.

Inverse transform sampling (100% efficient)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. note:: This is the method implemented in Neper.

The ODF is defined over (multidimensional) orientation space; however, the discretization of orientation space (its mesh elements) provides a unidimensional ODF function that depends on the orientations (the numbers of the mesh elements), :math:`f_u (x)`, where :math:`x` is an integer than varies from 1 to :math:`N` (the total number of elemental orientations).  (Note that the respective positions if the elements have no influence.) The "frequency of occurrence" of an orientation represented by element :math:`x`, which can be seen as a *probability*, :math:`p_x`, is given by:

.. math::

   \begin{equation}
     p_x = \frac{n_c}{\pi^2} \, v_x \, f_u(x)
   \end{equation}

where :math:`v_x` is the volume of orientation space that orientation :math:`x` represents (taking into account the orientation multiplicity), and :math:`f_u(x)` its ODF value, and :math:`n_c` is the multiplicity of the space (24 for cubic).

:math:`v_x` is computed by integration of :math:`dg` over the associated element.  In practice, it is taken as the volume of the element multiplied by :math:`(1 / (1 + \rho^2))^2`, in Rodrigues space (:math:`\rho = \sqrt{{r_1}^2 + {r_2}^2 + {r_3}^2}` assumed constant). So, :math:`p_x` sums to 1.

Formally, the inverse transform sampling is based on the fact that, for any random variable :math:`X \in \mathbb{R}`, the random variable :math:`F_X^{-1}(U)` has the same probability density function as :math:`X`, where :math:`F_X^{-1}` is the generalized inverse of the cumulative distribution function :math:`F_X` of :math:`X` and :math:`U` is uniform on :math:`[0,\,1]`.

In the present case, discrete (random) variables are discussed. The cumulative distribution function can be expressed as

.. math::

   \begin{equation}
     P(x) = \sum_{j\leq x} p_j
   \end{equation}

which is a step function.  As a step function does not have an inverse, the concept of generalized inverse is of particular importance.

The generalized inverse of :math:`P` is:

.. math::

   \begin{equation}
     P^-(y) = \hbox{inf}\left\{x \in \mathbb{R} : P(x) \geq y\right\}, y \in \mathbb{R}
   \end{equation}

Note that entire ranges of values of :math:`y` now correspond to a single value of :math:`x`.

The procedure then is to draw a random number between 0 and 1, :math:`y`, and compute the value of :math:`P^-(y)`.  This value is the element from which we generate an orientation.

To complete the process, for any selected element, :math:`y`, the orientation is chosen randomly within the element.  To do so, we pick a random position inside the element, following a uniform space distribution, using Rocchini and Cignoni's method (https://doi.org/10.1080/10867651.2000.10487528), which provides the orientation.

This method is direct and does not involve any testing or rejection.  It is *almost* correct in the sense that steps 1 and 2 are correct, but step 3 assumes a uniform density of the space inside an element.  This is reasonable assumption for properly conditioned spaces (such as Rodrigues space) and properly refined meshes, and is the same assumption as the one made to discretize the ODF itself.

Useful references:
  - https://link.springer.com/article/10.1007/s00186-013-0436-7
  - Morawiec's book for the rejection method
  - L13-Grain_Bndries_RFspace-15Mar16.pdf for the volume element
  - find a book in statistics for rejection and inverse sampling methods, which are standard
