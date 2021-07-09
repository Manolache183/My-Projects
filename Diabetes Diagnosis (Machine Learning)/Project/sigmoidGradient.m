function g = sigmoidGradient(z)
%Calculeaza gradientul.

g = zeros(size(z));

g = sigmoid(z) .* (1-sigmoid(z));

end
