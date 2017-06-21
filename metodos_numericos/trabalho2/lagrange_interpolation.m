function pn = lagrange_interpolation(X,Y,n,points)
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
    pn_str='@(x)';
    for k = 1:(n+1)
      Lk_str = '1';
      for j = 1:(n+1)
         if j ~= k
          Lk_str=strcat(Lk_str,'*((x-(',num2str(X(points(j))),'))/(',num2str(X(points(k))),'-(',num2str(X(points(j))),')))');
         end
      end
      pn_str=strcat(pn_str,'+(',Lk_str,'*',num2str(Y(points(k))),')');
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
  disp(strcat("\nPn(x)=",func2str(pn)));
  x = input("\nDigite um valor para x (vazio para sair): ",'s');
  while (~strcmp(x,''))
    x = eval(x);
    disp(strcat("\nPn(",num2str(x),')=',num2str(pn(x))));
    disp(strcat("\nPn(x)=",func2str(pn)));
    x = input("\nDigite um valor para x (vazio para sair): ",'s');
  end
end

[X, Y] = get_XY;
[n, points] = get_n_points;
pn = lagrange_interpolation(X,Y,n,points);
test_x(pn);

