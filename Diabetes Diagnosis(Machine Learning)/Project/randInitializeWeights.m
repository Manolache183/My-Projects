function W = randInitializeWeights(L_in, L_out)
% Initializam parametrii la intamplare, cu valori mici.
W = zeros(L_out, 1 + L_in);
 
epsilon_init = 0.12; 

W = rand(L_out, 1 + L_in) * 2 * epsilon_init - epsilon_init; 

end
