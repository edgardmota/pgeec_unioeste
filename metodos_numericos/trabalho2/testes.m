function valid = check_points(X,n,points)
  X_size = size(X);
  points_size = size(points);
  max_index = max(points);
  min_index = min(points);
  count = zeros(points_size);
  for i = 1:points_size
    count(i) = sum(points==points(i));
  end
  valid = (max_index <= X_size) & (min_index <= 1) & (points_size == sum(count(i))) & (points_size == (n + 1));
end

function n = get_n_size(X)
  n = size(X,2) - 1;
end

function X = gauss(A, b)
  X = A\b;
end

function valid = valid_inputs(X, Y)
  valid = isvector(X) & isvector(Y) & (size(X) == size(Y));
end

function pn = polynomial_interpolation(X,Y,n,points)
  valid_args = false;
  if valid_inputs(X,Y)
    if ~(exist('n','var'))
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
    disp('valido');
    A = zeros(n+1);
    for i = 1:(n+1)
      for j = 1:(n+1)
          A(i, j) = X(points(i))^(j-1);
      end
    end
    b = Y';
    a = gauss(A, b);
    pn_str = '';
    for i = 1:size(a,1)
      term_str = strcat('+(', num2str(a(i,1)),'*x^',num2str(i-1),')');
      pn_str = strcat(pn_str,term_str);
    end
    pn = eval(strcat('@(x)',pn_str));
  else
    error('Erro: argumentos invalidos!');
  end
end

function [X, Y] = get_XY
  %X = input('Entre com o vetor X: ');
  X = [-1 0 2];
  %Y = input('Entre com o vetor Y: ');
  Y = [4 1 -1];
end

[X, Y] = get_XY
pn = polynomial_interpolation(X,Y)
