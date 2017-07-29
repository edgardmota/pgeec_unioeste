pkg load symbolic

function integral = simpson_one_third_method(f,ab,e_sr)
  b = ab(2);
  a = ab(1);
  integral = function_handle(f)(a) + function_handle(f)(b);
  h = calc_h(e_sr,f,ab);
  c = a + h;
  i = 1;
  while(~almost_equal(c,b) & (c<b))
    if(mod(i,2)==0)
      m = 2;
    else
      m = 4;
    end
    i += 1;
    integral += m*function_handle(f)(c);
    c += h;
  end
  integral *= h/3;
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

function M4 = calc_M4(f,ab)
  df = diff_n(f,4);
  n = get_size(ab);
  M4 = function_handle(df)(ab(1));
  for i = 2:n
    r = function_handle(df)(ab(i));
    if (r > M4)
      M4 = r;
    end
  end
end

function h = calc_h(e_sr,f,ab)
  b = ab(2);
  a = ab(1);
  h = sqrt(sqrt(180*e_sr/((b-a)*calc_M4(f,ab))));
end

function s = get_size(X)
  s = size(X,2);
end

function e_sr = get_error
  e_sr = input("\nEntre com o erro desejado: ");
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
e_sr = get_error;
integral = simpson_one_third_method(f,ab,e_sr);
disp("\nO valor da integral é: "),disp(integral);
