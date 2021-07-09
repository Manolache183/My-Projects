function checkNNGradients(lambda)
%Cream o mica retea neurala pt a verifica implementarea algoritmului "Backpropagation".

if ~exist('lambda', 'var') || isempty(lambda)
    lambda = 0;
end

input_layer_size = 3;
hidden_layer_size = 5;
num_labels = 3;
m = 5;

% random data
Theta1 = debugInitializeWeights(hidden_layer_size, input_layer_size);
Theta2 = debugInitializeWeights(num_labels, hidden_layer_size);
% Generam un X random
X  = debugInitializeWeights(m, input_layer_size - 1);
y  = 1 + mod(1:m, num_labels)';

% Rulam parametrii
nn_params = [Theta1(:) ; Theta2(:)];

% Functia de referire
costFunc = @(p) nnCostFunction(p, input_layer_size, hidden_layer_size, ...
                               num_labels, X, y, lambda);

[cost, grad] = costFunc(nn_params);
numgrad = computeNumericalGradient(costFunc, nn_params);

% Examinam visual rezultatele
disp([numgrad grad]);
fprintf(['Coloanele de sus ar trebui sa fie similare\n' ...
         '(Stanga - Gradient Numeric, Right - Gradient Analitic)\n\n']);

% Evaluam eroarea medie dintre cele doua solutii.  
% Diferenta trebuie sa fie foarte mica.
diff = norm(numgrad-grad)/norm(numgrad+grad);

fprintf(['Daca implementarea algoritmului "Backpropagation" este corecta, \n' ...
         'diferenta relativa ar trebui sa fie mica (mai mica decat 1e-9). \n' ...
         '\nDiferenta Relativa: %g\n'], diff);

end
