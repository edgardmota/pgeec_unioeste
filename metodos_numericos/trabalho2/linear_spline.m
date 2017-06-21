function S = linear_spline(X,Y)
  if valid_inputs(X,Y)
    n = get_n_size(X);
    S_str='@(x)';
    for i = 1:n
      interval_str = sprintf('((x>%0.5f) & (x<=%0.5f)) .* ',X(i),X(i+1));
      sik = sprintf('((%0.5f)*(%0.5f-(x))+(%0.5f*(x-(%0.5f))))/(%0.5f - %0.5f)',X(i),X(i+1),X(i+1),X(i),X(i+1),X(i));
      S_str=strcat(S_str,'+',interval_str,sik);
    end
    S = eval(S_str);
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

function test_x(S)
  disp(strcat("\S(x)=",func2str(S)));
  x = input("\nDigite um valor para (x) ou 'sair': ",'s');
  while (~strcmp(x,'sair'))
    x = eval(x);
    disp(strcat("\nS(",num2str(x),')=',num2str(S(x))));
    disp(strcat("\nS(x)=",func2str(S)));
    x = input("\nDigite um valor para (x) ou 'sair': ",'s');
  end
end

[X, Y] = get_XY;
S = linear_spline(X,Y);
test_x(S);