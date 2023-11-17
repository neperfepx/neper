.. _1dmeshing:

1-D Laguerre Tessellation
=========================

A Voronoi tessellation use an Euclidean distance, which in 1D is

.. math::

  \begin{equation}
  {d_E(p_i,\,q)}^2 = (p_i - q)^2.
  \end{equation}

In Laguerre geometry, the distance is

.. math::

  \begin{equation}
  {d_L(p_i,\,q)}^2 = {d_E(p_i,\,q)}^2 - w_i.
  \end{equation}

The interface between two points :math:`p_i` and :math:`p_j`, of coordinate :math:`q`, is such
that

.. math::

  \begin{equation}
  {d_L(p_i,\,q)}^2 = {d_L(p_j,\,q)}^2,
  \end{equation}

which can be rewritten as

.. math::

  \begin{equation}
  {d_E(p_i,\,q)}^2 - w_i = {d_E(p_j,\,q)}^2 - w_j,
  \end{equation}

and so

.. math::

  \begin{equation}
  {(p_i - q)}^2 - w_i = {(p_j - q)}^2 - w_j,
  \end{equation}

.. math::

  \begin{equation}
  {p_i}^2 - 2\,p_i\,q + q^2 - w_i
  = {p_j}^2 - 2\,p_j\,q + q^2 - w_j,
  \end{equation}

.. math::

  \begin{equation}
  -2\,(p_i-p_j)\,q
  =   w_i - w_j
   - ({p_i}^2 - {p_j}^2),
  \end{equation}

.. math::

  \begin{equation}
  q
  = \frac{1}{2}\,(p_i + p_j)
  - \frac{1}{2}\,\frac{w_i - w_j}
        {p_i-p_j}.
  \end{equation}

It follows that

.. math::

  \begin{equation}
  q - p_i
  = \frac{1}{2}\,(p_j - p_i)
  - \frac{1}{2}\,\frac{w_i - w_j}
        {p_i-p_j},
  \end{equation}

or

.. math::

  \begin{equation}
  q - p_i
  = \frac{1}{2}\,(p_j - p_i)
  - \frac{1}{2}\,\frac{w_j - w_i}
        {p_j-p_i}.
  \end{equation}

