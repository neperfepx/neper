.. _regularization:

Regularization: Positioning a New Vertex
========================================

Deleting a tessellation edge implies merging its two vertices into one. From the topology point of view, the resulting vertex must belong to the faces of the two original vertices.  However, in the general case, it cannot lie within the planes of all of the faces.  This results in the faces being distorted, and the new vertex is positioned so that the sum of the squares of the distances to the original planes of the faces is minimum:

.. math::

  \begin{equation}
  {D} \left(\mathbf{{x^v}}\right)
    = \sum_{i=1}^{N} \left( \mathbf{n}_i \cdot \mathbf{{x^v}} - d_i \right)^2
    \quad \hbox{minimum.}
  \end{equation}

where :math:`\mathbf{{x^v}}` is the position of the vertex, :math:`N` is the number of faces the vertex belongs to and :math:`\mathbf{n_i} \cdot \mathbf{x} -d_i = 0` is the equation of face number :math:`i`.

The faces that are at the boundary of the domain are constrained to retain in their original planes.  Therefore, a vertex can also be subjected to the constraint that it must retain on a given plane.  This can be expressed as follows:

.. math::

  \begin{equation}
  \mathbf{n}_i^\prime \cdot \mathbf{{x^v}} - d_i^\prime = 0 \quad \hbox{ for
  }i\in\left\{1,\,2,\,\dots, M\right\},
  \end{equation}

where :math:`M` is the number of domain faces the vertex must retain on and :math:`\mathbf{n_i^\prime} \cdot \mathbf{x} -d_i^\prime = 0` are the plane equations.

This is a constraint minimization problem, which can be solved using Lagrange multipliers.

We define the following function:

.. math::
  \begin{equation}
  {G} \left(\mathbf{{x^v}},\,\lambda_i\right) = {D} \left(\mathbf{{x^v}}\right) +
  \sum_{i=1}^M \lambda_i
  \left(\mathbf{n}_i^\prime \cdot \mathbf{{x^v}} - d_i^\prime\right),
  \end{equation}

which by replacing :math:`{D}\left(\mathbf{{x^v}}\right)` by its expression becomes

.. math::
  \begin{equation}
  {G} \left(\mathbf{{x^v}},\,\lambda_i\right) =
  \sum_{i=1}^{N} \left( \mathbf{n}_i \cdot \mathbf{{x^v}} - d_i \right)^2
  + \sum_{i=1}^M \lambda_i
  \left(\mathbf{n}_i^\prime \cdot \mathbf{{x^v}} - d_i^\prime\right).
  \end{equation}

The derivative of the function with respect to :math:`\mathbf{{x^v}}` and each of the :math:`\lambda_i` must be zero, which gives,

.. math::
  :label: dgdx

  \begin{equation}
  \frac{1}{2}\,\frac{\partial\, {G}\left(\mathbf{{x^v}},\,\lambda_i\right) }{\partial \, \mathbf{{x^v}}}
  = 
  \sum_{i=1}^{N} \left(\mathbf{n}_i \otimes \mathbf{n}_i\right) \cdot \mathbf{{x^v}}
  - \sum_{i=1}^{N} d_i\,\mathbf{n}_i
  + \sum_{i=1}^M \frac{\lambda_i}{2} \, \mathbf{n}_i^\prime
    = \mathbf{0}
    \quad \hbox{(3 equations)}
  \end{equation}

and

.. math::
  :label: dgdlambda

  \begin{equation}
  \frac{\partial\, {G}\left(\mathbf{{x^v}},\,\lambda_i\right) }{\partial
    \, \lambda_i}
  = 
  \mathbf{n}_i^\prime \cdot \mathbf{{x^v}} - d_i^\prime = 0
  \quad \forall \, i\in\left\{1,\,2,\,\dots, M\right\}
  \quad \hbox{($M$ equations)}
  \end{equation}

which is the expression of the original constraint (as expected).

Equations :eq:`dgdx` and :eq:`dgdlambda` form a system of :math:`3+M` equations for :math:`3+M` unknowns (3 :math:`\mathbf{{x^v}}` components and :math:`M` :math:`\lambda_i`).  The system can be written as a linear matrix system.  With :math:`\mathbf{{N}} = \sum_{i=1}^N \left(\mathbf{n}_i \otimes \mathbf{n}_i\right)`,

.. math::
  \begin{equation}
  \left[\begin{array}{cccccc}
  {N}_{11} & {N}_{12} & {N}_{13} & (n_1^\prime)_1 & \cdots & (n_M^\prime)_1  \\
  {N}_{21} & {N}_{22} & {N}_{23} & (n_1^\prime)_2 & \cdots & (n_M^\prime)_2  \\
  {N}_{31} & {N}_{32} & {N}_{33} & (n_1^\prime)_3 & \cdots & (n_M^\prime)_3  \\
    (n_1^\prime)_1 & (n_1^\prime)_2 & (n_1^\prime)_3 & 0 & \cdots & 0 \\
    \cdots & \cdots & \cdots & 0 & \cdots & 0 \\
    (n_M^\prime)_1 & (n_M^\prime)_2 & (n_M^\prime)_3 & 0 & \cdots & 0 \\
  \end{array}\right]
  \left[\begin{array}{c}
  {x^v}_1 \\
  {x^v}_2 \\
  {x^v}_3 \\
  \frac{1}{2}\,\lambda_1 \\
  \cdots \\
  \frac{1}{2}\,\lambda_M \\
  \end{array}\right]
  =
  \left[\begin{array}{c}
  \sum_{i=1}^N d_i\,(n_i)_1 \\
  \sum_{i=1}^N d_i\,(n_i)_2 \\
  \sum_{i=1}^N d_i\,(n_i)_3 \\
    d_1^\prime \\
    \cdots     \\
    d_M^\prime \\
  \end{array}\right].
  \end{equation}
