pkg load symbolic;

function [xi, yi] = heun_method(f,x0_y0,h_xn)
  h = h_xn(1);
  xn = h_xn(2);
  x0 = x0_y0(1);
  y0 = x0_y0(2);

  x = x0;
  y = y0;
  y_new = y0;
  yi = [];
  xi = [];
  while(x <= xn)
    xi = vertcat(xi,[x]);
    yi = vertcat(yi,[y]);

    y0_new = y + function_handle(f)(x,y)*h;
    y_new = y + (function_handle(f)(x,y) + function_handle(f)(x+h,y0_new))/2*h;
    x += h;
    y = y_new;
  end
end

function s = get_size(X)
  s = size(X,2);
end

function f = get_f
  syms x y;
  f = input("\nEntre com a funçao f(x,y): ");
end

function h_xn = get_step_xn
  h_xn = zeros(1,2);
  h_xn(1) = input("\nEntre com passo (h): ");
  h_xn(2) = input('Entre com o valor de xn: ');
end

function x0_y0 = get_initial_values
  x0_y0 = zeros(1,2);
  x0_y0(1) = input("\nEntre com o valor de x0: ");
  x0_y0(2) = input('Entre com o valor de y0: ');
end

f = get_f;
x0_y0 = get_initial_values;
h_xn = get_step_xn;
[xi, yi] = heun_method(f,x0_y0,h_xn);
disp("\nOs valores são: "),disp(horzcat(xi, yi));
