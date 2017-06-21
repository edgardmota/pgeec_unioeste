function phi = least_squares_continuous(i_a,i_b,f,gs)
  if valid_inputs(i_a,i_b)
    n = get_size(gs);
    a = zeros(n);
    b = zeros(n,1);
    for i = 1:n
      g_str = func2str(gs{i})(5:end);
      f_str = func2str(f)(5:end);
      m_str = strcat('@(x)',f_str,'*',g_str);
      b(i) = quad(eval(m_str),i_a,i_b);
      for  j = 1:n
        gi_str = func2str(gs{i})(5:end);
        gj_str = func2str(gs{j})(5:end);
        m_str = strcat('@(x)',gi_str,'*',gj_str);
        a(i,j) = quad(eval(m_str),i_a,i_b);
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

function valid = valid_inputs(a, b)
  valid = a < b;
end

function [a, b] = get_ab
  disp("Intervalo de [a,b]:\n");
  a = input('Entre com o valor de a: ');
  b = input('Entre com o valor de b: ');
end

function gs = get_gs
  i = 1;
  f = input(sprintf('Entre com a funçao g_%d(x) (vazio para sair): ',i),'s');
  while (~strcmp(f,''))
    gs{end+1} = eval(f);
    f = input(sprintf('Entre com a funçao g_%d(x) (vazio para sair): ',i),'s');
  end
end

function f = get_f
  f = input('Entre com a funçao f(x): ');
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

f = get_f;
gs = get_gs;
[a, b] = get_ab;
phi = least_squares_continuous(a,b,f,gs);
test_x(phi);