function s = linear_spline(X,Y)
  if valid_inputs(X,Y)
    n = get_n_size(X);
    disp('f = @(x) (x>0) .* x + (x<0) .*-1*x');
    for i = 2:n+1
      pn_str=strcat('+(',d(1,i),')*(x-(',num2str(n_X(i-1)),'))');
    end
    pn = eval(pn_str);
  else
    error('Erro: argumentos invalidos!');
  end
end

function n = get_n_size(X)
  n = size(X,2) - 1;
end

function valid = valid_inputs(X, Y)
  valid = isvector(X) & isvector(Y) & (size(X) == size(Y));
end

function [X, Y] = get_XY
  X = input('Entre com o vetor X: ');
  Y = input('Entre com o vetor Y: ');
end

function test_x(pn)
  disp(strcat("\npn(x)=",func2str(pn)));
  x = input("\nDigite um valor para (x) ou 'sair': ",'s');
  while (~strcmp(x,'sair'))
    x = eval(x);
    disp(strcat("\nPn(",num2str(x),')=',num2str(pn(x))));
    disp(strcat("\npn(x)=",func2str(pn)));
    x = input("\nDigite um valor para (x) ou 'sair': ",'s');
  end
end

[X, Y] = get_XY;
pn = spline_linear(X,Y);
test_x(pn);

