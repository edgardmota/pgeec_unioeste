function pn = polynomial_interpolation(X,Y,d,n,points);
  valid_args = false;
  if valid_inputs(X,Y)
      if (check_points(X,n,points))
        valid_args = true;
      end
  end
  if valid_args
    A = zeros(n+1);
    for i = 1:(n+1)
      for j = 1:(n+1)
          A(i, j) = X(points(i))^(j-1);
      end
    end
    b = zeros(n+1,1);
    for i = 1:(n+1)
      b(i) = Y(points(i));
    end
    a = gauss(A, b);
    pn_str = '@(x)';
    for i = 1:size(a,1)
      term_str = strcat('+(', num2str(a(i,1)),'*x^',num2str(i-1),')');
      pn_str = strcat(pn_str,term_str);
    end
    pn = eval(pn_str);
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

function n = get_size(X)
  n = size(X,2) - 1;
end

function X = gauss(A, b)
  X = A\b;
end

function valid = valid_inputs(X, Y)
  valid = isvector(X) & isvector(Y) & (size(X) == size(Y));
end

function [X, Y] = get_XY
  X = input('Entre com o vetor X: ');
  Y = input('Entre com o vetor Y: ');
end

function m = max_dd_n_p_1(d,n)
  pos = n + 2;
  if (pos > size(d,1)) 
    pos = pos - 1
  end
  l = reshape(d(:,pos),1,size(d,1));
  for i = 1:n+1
    l(i) = abs(l(i));
  end
  m = max(l);
end

function enx = estimated_error(x,X,d,n,points)
  enx = max_dd_n_p_1(d,n);
  for i = 1:n+1
    enx = enx*(x-X(points(i)));
  end
  enx = abs(enx);
end

function d = divided_differences(X, Y)
  valid_args = false;
  if (nargin == 2)
      [p , n] = size(X);
      if ((p == 1) & (p ==size(Y, 1)) & (n == size(Y, 2)))
        valid_args = true;
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

function [n, points] = get_n_points
  n = input('Grau do polinomio aproximador (n) (vazio para grau maximo): ');
  if (size(n,1) ~= 0)
    point = input(sprintf('Indice de X a partir do qual usar %d pontos: ',n + 1));
    points = [point:point+n];
  else
    n = 0;
    points = [];
  end
end

function test_x(pn,X,d,n,points)
  disp(strcat("\npn(x)=",func2str(pn)));
  x = input("\nDigite um valor para x (vazio para sair): ",'s');
  while (~strcmp(x,''))
    x = eval(x);
    disp(strcat("\nPn(",num2str(x),')=',num2str(pn(x)),' (Erro aproximado:  ',num2str(estimated_error(x,X,d,n,points)),')'));
    disp(strcat("\npn(x)=",func2str(pn)));
    x = input("\nDigite um valor para x (vazio para sair): ",'s');
  end
end

[X, Y] = get_XY;
[n, points] = get_n_points;
if (n == 0)
  n = get_size(X);
  points = [1:n+1];
end
d = divided_differences(X,Y);
pn = polynomial_interpolation(X,Y,d,n,points);
test_x(pn,X,d,n,points);
