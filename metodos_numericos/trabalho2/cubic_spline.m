function S = cubic_spline(X,Y)
  if valid_inputs(X,Y)
    points_per_base = 5;
    n = get_n_size(X);
    h=zeros(1,n);
    for k = 1:n
        h(k) = X(k+1) - X(k);
    end;
    functions = size(X,2) - points_per_base + 1;
    S_str = '@(x)';
    interval_str = sprintf('((x>%0.5f) & (x<=%0.5f)) .* ',X(1),X(2));
    for i = 1:functions
      g = zeros(points_per_base);
      b = zeros(points_per_base,1);
      for k = 1:points_per_base
          for j = 1:points_per_base
            g(k,j) = 0;
          end
        if ((k == 1)||(k == points_per_base))
          g(k,k) = 1;
          b(k,1) = 0;
        else
          g(k,k-1) = h(k);
          g(k,k) = 2*(h(k-1)+h(k));
          g(k,k+1) = h(k);
          b(k,1) = 6*((Y(k+1)-Y(k))/h(k)-(Y(k)-Y(k-1))/h(k-1));
        end
      end
      g = gauss(g,b);
      a = (g(2) - g(1))/(6*h(i));
      b = g(2)/2;
      c = ((Y(i+1)-Y(i))/h(i) + (2*h(i)*g(2)+g(1)*h(i))/6);
      d = Y(i+1);
      si = sprintf('(%0.5f*(x-(%0.5f))^ 3)+(%0.5f*(x-(%0.5f))^ 2)+(%0.5f*(x-(%0.5f)))+(%0.5f)',a,X(i+1),b,X(i+1),c,X(i+1),d);
      S_str = strcat(S_str,interval_str,si);
      interval_str = sprintf('+ ((x>%0.5f) & (x<=%0.5f)) .* ',X(1+i),X(2+i));
    end
    S = eval(S_str);
  else
    error('Erro: argumentos invalidos!');
  end
end

function X = gauss(A, b)
  X = A\b;
end

function n = get_n_size(X)
  n = size(X,2) - 1;
end

function valid = valid_inputs(X, Y)
  valid = isvector(X) & isvector(Y) & (size(X) == size(Y)) & (size(X,2)>=5);
end

function [X, Y] = get_XY
  X = input('Entre com o vetor X: ');
  Y = input('Entre com o vetor Y: ');
end

function test_x(S)
  disp(strcat("n\S(x)=",func2str(S)));
  x = input("\nDigite um valor para x (vazio para sair): ",'s');
  while (~strcmp(x,''))
    x = eval(x);
    disp(strcat("\nS(",num2str(x),')=',num2str(S(x))));
    disp(strcat("\nS(x)=",func2str(S)));
    x = input("\nDigite um valor para x (vazio para sair): ",'s');
  end
end

[X, Y] = get_XY;
S = cubic_spline(X,Y);
test_x(S);