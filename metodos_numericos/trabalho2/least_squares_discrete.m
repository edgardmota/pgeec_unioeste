function phi = least_squares_discrete(X,Y,gs)
  if valid_inputs(X,Y)
    m = get_size(X);
    n = get_size(gs);
    a = zeros(n);
    b = zeros(1,n);
    for i = 1:n
      for k = 1:m 
          b(i) = b(i) + Y(k)*gs{i}(X(k));
      end
      for  j = 1:n
        a(i,j) = 0;
        for k = 1:m 
          a(i,j) = a(i,j) + gs{i}(X(k))*gs{j}(X(k));
        end
      end
    end
    alpha = gauss(a,b);
    phi_str = '@(x)';
    for i = 1:n
      phi_str = strcat(phi_str,'+',num2str(alpha(i)),'*',func2str(gs{i})(5:end));
    end
    phi = eval(phi_str);
  else
    error('Erro: argumentos invalidos!');
  end
end

function X = gauss(A, b)
  X = A\b;
end

function s = get_size(X)
  s = size(X,2);
end

function valid = valid_inputs(X, Y)
  valid = isvector(X) & isvector(Y) & (size(X) == size(Y));
end

function [X, Y] = get_XY
  X = input('Entre com o vetor X: ');
  Y = input('Entre com o vetor Y: ');
end

function gs = get_gs
  i = 1;
  f = input(sprintf('Entre com a funçao g_%d(x) (vazio para sair): ',i),'s');
  while (~strcmp(f,''))
    gs{end+1} = eval(f);
    f = input(sprintf('Entre com a funçao g_%d(x) (vazio para sair): ',i),'s');
  end
end

function test_x(phi)
  disp(strcat("\nphi(x)=",func2str(phi)));
  x = input("\nDigite um valor para x (vazio para sair): ",'s');
  while (~strcmp(x,''))
    x = eval(x);
    disp(strcat("\nphi(",num2str(x),')=',num2str(phi(x))));
    disp(strcat("\nphi(x)=",func2str(phi)));
    x = input("\nDigite um valor para x (vazio para sair): ",'s');
  end
end

[X, Y] = get_XY;
gs = get_gs;
phi = least_squares_discrete(X,Y,gs);
test_x(phi);