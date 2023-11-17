.. _smoothing:

Constrained Laplacian Smoothing
===============================

The usual Laplacian smoothing algorithm is an iterative algorithm which consists in moving each node depending on the position of its neighbors. This can be expressed as

.. math::

  \begin{equation}
  \mathbf{x}^{a+1} = (1 - A) \, \mathbf{x}^{a} + A \, \mathbf{x_n}^{a},
  \end{equation}

where :math:`\mathbf{x}^{a+1}` is the position of the node at iteration :math:`a+1` (accordingly, :math:`\mathbf{x}^{a}` is the position of the node at iteration :math:`a`), :math:`\mathbf{x_n}^{a}` is a position depending on the neighbors (at iteration :math:`a`) and :math:`A` is a weighting factor.

In standard Laplacian algorithm, :math:`\mathbf{x_n}^{a}` is taken as the barycenter of the neighbors:

.. math::
  :label: eqn:xnabary

  \begin{equation}
    \mathbf{x_n}^{a} = \frac{\sum_{i=1}^N {\alpha_i}^a\,\mathbf{x_i}^{a}}
                          {\sum_{i=1}^N {\alpha_i}^a},
  \end{equation}

where :math:`{\alpha_i}^{a}` and :math:`\mathbf{x_i}^{a}` are the weight and position of neighbor :math:`i` of the node submitted to smoothing (position :math:`\mathbf{x}^{a}`). :math:`{\alpha_i}^{a}` can be defined as the inverse of the distance between :math:`\mathbf{x_i}^a` and :math:`\mathbf{x}^a`.

In our case, however, a node which is on the domain boundary must be constrained to retain within the faces it belongs to.  :math:`\mathbf{x}^{a}` satisfies the constraints and :math:`\mathbf{x}^{a+1}` must do too.  This is ensured by reporting the constraints on :math:`\mathbf{x_n}^{a}`.  Its definition (Equation :eq:`eqn:xnabary`) must be changed to accept constraints, which can typically be done by turning to minimization.

Equation :eq:`eqn:xnabary` can be rewritten as

.. math::
  :label: eqn:xnabaryrw

  \begin{equation}
    \sum_{i=1}^N {\alpha_i}^a\,\left(\mathbf{x_i}^{a} - \mathbf{x_n}^{a}\right) = \mathbf{0}.
  \end{equation}

To allow for constraints, we can turn Equation :eq:`eqn:xnabaryrw` into,

.. math::
  :label: eqn:xnabaryrw2

  \begin{equation}
    \left|\sum_{i=1}^N {\alpha_i}^a\,\left(\mathbf{x_i}^{a} - \mathbf{x_n}^{a}\right)\right| \quad \hbox{minimum.}
  \end{equation}

Such an expression is not well suited to (analytic) minimization.  We can instead consider a quadratic expression, as follows:

.. math::
  :label: eqn:xnabaryrw2

  \begin{equation}
    \mathcal{D}\left(\mathbf{x_n}^a\right)
    = \sum_{i=1}^N {\alpha_i}^a\,\left(\mathbf{x_i}^{a} - \mathbf{x_n}^{a}\right)^2 \quad \hbox{minimum}.
  \end{equation}

:math:`\mathbf{x_n}^a` must be constrained to retain in the planes of the domain faces the node submitted to smoothing belongs to.  This can be expressed as

.. math::

  \begin{equation}
  \mathbf{n}_i^\prime \cdot \mathbf{{x_n}}^a - d_i^\prime = 0 \quad \hbox{ for
  }i\in\left\{1,\,2,\,\dots, N\right\},
  \end{equation}

where :math:`N` is the number of domain faces and :math:`\mathbf{n_i^\prime} \cdot \mathbf{x} -d_i^\prime = 0` are the plane equations.

This is a constraint minimization problem, which can be solved using Lagrange multipliers.

We define the following function:

.. math::

  \begin{equation}
  \mathcal{G} \left(\mathbf{{x_n}}^a,\,\lambda_i\right) = \mathcal{D}
  \left(\mathbf{{x_n}}^a\right) +
  \sum_{i=1}^N \lambda_i
  \left(\mathbf{n}_i^\prime \cdot \mathbf{{x_n}}^a - d_i^\prime\right),
  \end{equation}

which by replacing :math:`\mathcal{D}\left(\mathbf{{x_n}}^a\right)` by its expression becomes

.. math::

  \begin{equation}
  \mathcal{G} \left(\mathbf{{x_n}}^a,\,\lambda_i\right) =
  \sum_{i=1}^N {\alpha_i}^a\,\left(\mathbf{x_i}^{a} - \mathbf{x_n}^{a}\right)^2
  + \sum_{i=1}^M \lambda_i
  \left(\mathbf{n}_i^\prime \cdot \mathbf{{x_n}}^a - d_i^\prime\right).
  \end{equation}

The derivative of the function with respect to :math:`\mathbf{{x_n}}^a` and each of the :math:`\lambda_i` must be zero, which gives

.. math::
  :label: eq:dgdx

  \begin{equation}
  \frac{\partial\, \mathcal{G}\left(\mathbf{{x_n}}^a,\,\lambda_i\right)
  }{\partial \, \mathbf{{x_n}}^a}
  = 
  -2\,\sum_{i=1}^{N} {\alpha_i}^a\,\left(\mathbf{x_i}^a - \mathbf{x_n}^a \right) 
  + \sum_{i=1}^M \lambda_i \, \mathbf{n}_i^\prime
    = \mathbf{0}
    \quad \hbox{(3 equations)},
  \end{equation}

and

.. math::
  :label: eq:dgdlambda

  \begin{equation}
  \frac{\partial\, \mathcal{G}\left(\mathbf{{x_n}}^a,\,\lambda_i\right) }{\partial
    \, \lambda_i}
  = 
  \mathbf{n}_i^\prime \cdot \mathbf{{x_n}}^a - d_i^\prime = 0
  \quad \forall \, i\in\left\{1,\,2,\,\dots, M\right\}
  \quad \hbox{($M$ equations)},
  \end{equation}

which is the expression of the original constraint (as expected).

Equations :eq:`eq:dgdx` and :eq:`eq:dgdlambda` form a system of :math:`3+M` equations for :math:`3+M` unknowns (3 :math:`\mathbf{{x_n}}^a` components and :math:`M` :math:`\lambda_i`).  The system can be written as a linear matrix system:

.. math::

  \begin{equation}
  \left[\begin{array}{cccccc}
  \sum_{i=1}^N {\alpha_i}^a & 0 & 0 & (n_1^\prime)_1 & \cdots & (n_M^\prime)_1  \\
  0 & \sum_{i=1}^N {\alpha_i}^a & 0 & (n_1^\prime)_2 & \cdots & (n_M^\prime)_2  \\
  0 & 0 & \sum_{i=1}^N {\alpha_i}^a & (n_1^\prime)_3 & \cdots & (n_M^\prime)_3  \\
    (n_1^\prime)_1 & (n_1^\prime)_2 & (n_1^\prime)_3 & 0 & \cdots & 0 \\
    \cdots & \cdots & \cdots & 0 & \cdots & 0 \\
    (n_M^\prime)_1 & (n_M^\prime)_2 & (n_M^\prime)_3 & 0 & \cdots & 0 \\
  \end{array}\right]
  \left[\begin{array}{c}
  \left({x_n}^a\right)_1 \\
  \left({x_n}^a\right)_2 \\
  \left({x_n}^a\right)_3 \\
  \frac{1}{2}\,\lambda_1 \\
  \cdots \\
  \frac{1}{2}\,\lambda_M \\
  \end{array}\right]
  =
  \left[\begin{array}{c}
  \sum_{i=1}^N {\alpha_i}^a\,({x_i}^a)_1 \\
  \sum_{i=1}^N {\alpha_i}^a\,({x_i}^a)_2 \\
  \sum_{i=1}^N {\alpha_i}^a\,({x_i}^a)_3 \\
    d_1^\prime \\
    \cdots     \\
    d_M^\prime \\
  \end{array}\right].
  \end{equation}

If several domain faces have the same equation, the corresponding constraint must be taken into account as one equation only to avoid the above matrix to be singular.

