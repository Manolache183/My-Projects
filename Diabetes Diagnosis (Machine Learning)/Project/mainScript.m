%% Scriptul principal pentru realizarea, antrenarea si testarea retelei neurale.

%% Initializare
clear ; close all; clc

%% Setarea parametrilor retelei.
input_layer_size  = 8;  % 8 caracteristici ale unui pacient 
hidden_layer_size = 8;   % 8 unitati in stratul ascuns
num_labels = 1;          % un singur label ce poate fi 0 sau 1, sanatos respectiv bolnav  

%% =========== Partea 1: Incarcarea datelor =============
fprintf('Incarcam datele ...\n')

data1 = load('Training_set.csv'); % 691 de pacienti, fiecare cu 8 caracteristici analizate
data2 = load('Test_set.csv'); % 77 de pacienti, setul de test ale carui valori nu vor fi folosite pentru antrenat
X = data1(:, 1:8);
Y = data1(:, 9);
X_test = data2(:, 1:8);
Y_test = data2(:, 9);
m = size(X, 1);

X = featureNormalize(X);
X_test = featureNormalize(X_test);

%% ================ Partea 2: Sigmoid Gradient  ================
%  Aici trebuie implementata functia matematica ce sta la baza acestei retele neurale.

fprintf('\nEvaluam functia de calcul sigmoid gradient...\n')

g = sigmoidGradient([-1 -0.5 0 0.5 1]);
fprintf('Sigmoid gradient evaluat la [-1 -0.5 0 0.5 1]:\n  ');
fprintf('%f ', g);
fprintf('\n\n');

fprintf('Rezultatele ar trebui sa fie 0.196612 0.235004 0.250000 0.235004 0.196612.\n');
pause;


%% ================ Partea 3: Initializarea Parametrilor ================
%  Initializam parametrii ce trebuiesc folositi in retea.

fprintf('\nSe initializeaza parametrii retelei neurale ...\n')

initial_Theta1 = randInitializeWeights(input_layer_size, hidden_layer_size);
initial_Theta2 = randInitializeWeights(hidden_layer_size, num_labels);

% Rulam parametrii intr-un singur vector pentru a-i putea administra mai usor
initial_nn_params = [initial_Theta1(:) ; initial_Theta2(:)];


%% =============== Partea 4: Implementarea algoritmului "Backpropagation" ===============
% Aici verificam initial calculand prin doua metode niste parametrii
% ce apartin unei retele neurale mici, create doar pentru acest scop
% daca diferenta este foarte mica intre acesti parametrii,
% atunci implementarea algoritmului este corecta.
fprintf('\nVerificam Algoritmul... \n');

%  Apelam functia de verificare checkNNGradients
checkNNGradients;

%% =============== Partea 5: Implementam Regularizarea ===============
% Odata ce aflam ca algoritmul este corect, implementam regularizare,
% pentru ca algoritmul sa poata generaliza si pe exemple ce nu sunt in setul de antrenament
% astfel evitam "overfitting-ul".

fprintf('\nVerificam "Backpropagation-ul" (cu Regularizare) ... \n')

%  Verificam din nou, cu coeficientul de regularizare lambda = 3;
lambda = 3;
checkNNGradients(lambda);

% Scoatem pe ecran valorile aflate de debug ale functiei cost...
debug_J  = nnCostFunction(initial_nn_params, input_layer_size, ...
                          hidden_layer_size, num_labels, X, Y, lambda);

fprintf(['\n\nCost cu parametrii fixi (cu lambda = %f): %f ' ...
         '\n(Pentru lambda = 3, valoarea asta ar trebui sa fie in jur de 0.7)\n\n'], lambda, debug_J);


%% =================== Partea 6: Antrenam Reteaua Neurala ===================

fprintf('\nAntrenam Reteaua Neurala... \n')

options = optimset('MaxIter', 1000); %1000 de repetari pentru a o antrena, suficiente cat sa convearga la un minim global.

%  Incercam diferite valori ale coeficientului de regularizare.
lambda = 0.1;
costFunction = @(p) nnCostFunction(p, ...
                                   input_layer_size, ...
                                   hidden_layer_size, ...
                                   num_labels, X, Y, lambda);


[nn_params, cost] = fmincg(costFunction, initial_nn_params, options);

% Obtinem din nou parametrii Theta1 si Theta 2
Theta1 = reshape(nn_params(1:hidden_layer_size * (input_layer_size + 1)), ...
                 hidden_layer_size, (input_layer_size + 1));

Theta2 = reshape(nn_params((1 + (hidden_layer_size * (input_layer_size + 1))):end), ...
                 num_labels, (hidden_layer_size + 1));



%% ================= Partea 7: Implement Predict =================
% Dupa ce am antrenat reateaua vom afla acuratetea pe setul de antrenament
% si pe setul de test

pred1 = predict(Theta1, Theta2, X);
fprintf('\nAcuratetea pe setul de antrenament: %f\n', mean(double(pred1 == Y)) * 100);

pred2 = predict(Theta1, Theta2, X_test);
fprintf('\nAcuratetea pe setul de test: %f\n', mean(double(pred2 == Y_test)) * 100);


