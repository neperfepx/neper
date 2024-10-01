settings.outformat="pdf";

unitsize(3cm);

draw(shift((0.15,0.15))*scale(1.8,1.4)*polygon(4),invisible);

draw(scale(0.8)*polygon(6));
draw(circle((0,0),0.025), linewidth(0.8));

// First graph
draw((0,0)--(1,0), Arrow);
draw((0,0)--(0,1), Arrow);
label("$X_c$", (1,0), N);
label("$Y_c$", (0,1), E);
label("$Z_c$", (0,0), SW);
