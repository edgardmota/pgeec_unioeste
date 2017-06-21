function pn = newton_interpolation(X,Y,n,points)
  valid_args = false;
  if valid_inputs(X,Y)
    if (~(exist('n','var')) || (size(n,1) == 0) || (size(points,1) == 0))
      n = get_n_size(X);
      points = [1:n+1];
      valid_args = true;
    elseif exist('points','var')
      if (check_points(X,n,points))
        valid_args = true;
      end
    end
  end
  if valid_args
    n_X = zeros(1,n+1);
    y_X = zeros(1,n+1);
    for i = 1:(n+1)
      n_X(i) = X(points(i));
      n_Y(i) = Y(points(i));
    end
    d = (divided_differences( n_X, n_Y));
    pn_str = strcat('@(x)',num2str(d(1,1)));
    for i = 2:n+1
      pn_str=strcat('+(',d(1,i),')*(x-(',num2str(n_X(i-1)),'))');
    end
    pn = eval(pn_str);
  else
    error('Erro: argumentos invalidos!');
  end
end

function d = divided_differences(X, Y)
  valid_args = false;
  if (nargin == 2)
      [p , n] = size(X);
      if ((p == 1) & (p ==size(Y, 1)) & (n == size(Y, 2)))
        valid_args = true
      end
  end
  if valid_args
    d = zeros(n, n);
    d(:,1) = Y';
    for j = 2:n
        for i = 1:(n - j + 1)
            d(i,j) = (d(i + 1, j - 1) - d(i, j - 1)) / (X(i + j - 1) - X(i));
        end
    end
  else
    error('Erro: argumentos invalidos!');
  end
end

function valid = check_points(X,n,points)
  X_size = size(X,2);
  points_size = size(points,2);
  max_index = max(points);
  min_index = min(points);
  count = zeros(1,points_size);
  for i = 1:points_size
    count(i) = sum(points==points(i));
  end
  valid = (max_index <= X_size) & (min_index >= 1) & (points_size == sum(count)) & (points_size == (n + 1));
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

function [n, points] = get_n_points
  n = input('Grau do polinomio aproximador (n) (vazio para grau maximo): ');
  if (size(n,1) ~= 0)
    points = input('Vetor com os indices de X a serem usados: ');
  else
    points = [];
  end
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
[n, points] = get_n_points;
pn = polynomial_interpolation(X,Y,n,points);
test_x(pn);

