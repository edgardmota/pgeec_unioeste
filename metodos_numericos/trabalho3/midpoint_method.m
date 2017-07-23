pkg load symbolic;

function [xi, yi] = midpoint_method(f,x0_y0,h_xn)
  h = h_xn(1);
  xn = h_xn(2);
  x0 = x0_y0(1);
  y0 = x0_y0(2);

  x = x0;
  y = y0;

  yi = [];
  xi = [];
  while(x <= xn)
    xi = vertcat(xi,[x]);
    yi = vertcat(yi,[y]);

    k1 = function_handle(f)(x,y);
    k2 = function_handle(f)(x + h/2,y + k1*h/2);

    y_new = y + k2*h;
    x += h;
    y = y_new;
  end
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
[xi, yi] = midpoint_method(f,x0_y0,h_xn);
disp("\nOs valores são: "),disp(horzcat(xi, yi));
