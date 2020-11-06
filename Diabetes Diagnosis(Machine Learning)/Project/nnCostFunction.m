function [J grad] = nnCostFunction(nn_params, ...
                                   input_layer_size, ...
                                   hidden_layer_size, ...
                                   num_labels, ...
                                   X, y, lambda)

%Derulam Parametrii:
Theta1 = reshape(nn_params(1:hidden_layer_size * (input_layer_size + 1)), ...
                 hidden_layer_size, (input_layer_size + 1));

Theta2 = reshape(nn_params((1 + (hidden_layer_size * (input_layer_size + 1))):end), ...
                 num_labels, (hidden_layer_size + 1));

m = size(X, 1);
         
J = 0;
Theta1_grad = zeros(size(Theta1));
Theta2_grad = zeros(size(Theta2));

gamma1 = Theta1(:, 2:end);
gamma2 = Theta2(:, 2:end);
for w = 1 : num_labels
  Y(:, w) = y == w;
endfor

%% 1: FeedForward si J
a1 = [ones(m, 1) X]; 
z2 = a1 * Theta1';

a2 = sigmoid(z2);
a2 = [ones(m, 1), a2];
z3 = a2 * Theta2';

a3 = sigmoid(z3);

cost = sum((Y .* log(a3)) + ((1 - Y) .* log(1 - a3)), 2);
J = - 1/m * sum(cost) + (lambda/(2*m)) * (sum(sum(gamma1.^2)) + sum(sum(gamma2.^2)));

%%backpropagation


DeltaMare1 = 0;
DeltaMare2 = 0;

for t = 1 : m
  
  % Pasul 1 - Input Values
  a1 = X(t, :);
  a1 = [1, a1];
  z2 = a1 * Theta1';
  
  a2 = sigmoid(z2);
  a2 = [1, a2];
  z3 = a2 * Theta2';
  
  a3 = sigmoid(z3);
  
  %Pasul 2 - Delta Output Layer
  deltaMic3 = a3 - Y(t, :); 
  
  %Pasul 3 - Delta Hidden Layer
  deltaMic2 = (deltaMic3 * gamma2).*sigmoidGradient(z2);
  
  %Pasul 4 - Accumulate
  DeltaMare1 = DeltaMare1 + a1' * deltaMic2;
  DeltaMare2 = DeltaMare2 + a2' * deltaMic3;
endfor

%Pasul 5 - derivatele
DeltaMare1 = DeltaMare1';
DeltaMare2 = DeltaMare2';

Theta1_grad = (1/m) * DeltaMare1;
Theta2_grad = (1/m) * DeltaMare2;

%regularizat
Theta1_grad(:, 2:end) += ((lambda / m) * gamma1);
Theta2_grad(:, 2:end) += ((lambda / m) * gamma2);

% Unroll gradients
grad = [Theta1_grad(:) ; Theta2_grad(:)];


end
