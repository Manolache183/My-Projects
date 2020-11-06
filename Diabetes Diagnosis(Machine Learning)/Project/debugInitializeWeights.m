function W = debugInitializeWeights(fan_out, fan_in)
% O functie ce este necesara debug-ului.

W = zeros(fan_out, 1 + fan_in);

W = reshape(sin(1:numel(W)), size(W)) / 10;


end
