pkg load symbolic

function integral = trapezoidal_method(f,ab,e_tr)
  b = ab(2);
  a = ab(1);
  integral = function_handle(f)(a) + function_handle(f)(b);
  h = calc_h(e_tr,f,ab);
  c = a + h;
  while(~almost_equal(c,b) & (c<b))
    integral += 2*function_handle(f)(c);
    c += h;
  end
  integral *= h/2;
end

function r = almost_equal(x,y)
  r = (abs(x-y) < 10^-5);
end

function df = diff_n(f,n)
  df = f;
  for i = 1:n
    df = diff(df);
  end
end

function M2 = calc_M2(f,ab)
  df = diff_n(f,1);
  n = get_size(ab);
  M2 = function_handle(df)(ab(1));
  for i = 2:n
    r = function_handle(df)(ab(i));
    if (r > M2)
      M2 = r;
    end
  end
end

% function m = subintervals(e_tr,f,ab)
%   b = ab(2);
%   a = ab(1);
%   m = (b-a)/sqrt(12*e_tr/((b-a)*calc_M2(f,ab)));
% end

function h = calc_h(e_tr,f,ab)
  b = ab(2);
  a = ab(1);
  h = sqrt(12*e_tr/((b-a)*calc_M2(f,ab)));
end

function s = get_size(X)
  s = size(X,2);
end


function e_tr = get_error
  e_tr = input("\nEntre com o erro desejado: ");
end

function f = get_f
  syms x
  f = input('Entre com a funçao f(x): ');
end

function ab = get_ab
  ab = zeros(1,2);
  disp("\nIntervalo de [a,b]:\n");
  ab(1) = input('Entre com o valor de a: ');
  ab(2) = input('Entre com o valor de b: ');
end

f = get_f;
ab = get_ab;
e_tr = get_error;
integral = trapezoidal_method(f,ab,e_tr);
disp("\nO valor da integral é: "),disp(integral);
