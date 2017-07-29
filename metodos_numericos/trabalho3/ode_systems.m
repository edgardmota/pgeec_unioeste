pkg load symbolic;

function sol = ode_system(dy,ys,h_x0_xn)
  syms x y1;

  h = h_x0_xn(1);
  x0 = h_x0_xn(2);
  xn = h_x0_xn(3);

  n = get_size(dy);
  p = [dy(1)*h];
  for i=2:n
    syms(strcat('y',num2str(i)));
    p = vertcat(p,[dy(i)*h]);
  end
  q = [subs(dy(1),'x',sym(x)+h)*h];
  for i=2:n
    q = vertcat(q,[subs(dy(i),'x',sym(x)+h)*h]);
  end
  for i=1:n
    y_current_str = strcat('y',num2str(i));
    for j=1:n
      q(j) = subs(q(j),y_current_str,sym(y_current_str)+p(i));
    end
  end
  y_new = [sym('y1')+(p(1)+q(1))/2];
  for i=2:n
    y_current_str = strcat('y',num2str(i));
    y_new = vertcat(y_new,[sym(y_current_str)+(p(i)+q(i))/2]);
  end
  sol = zeros(1,n+1);
  sol(1,1) = x0;
  for i=2:n+1
    sol(1,i) = ys(i-1);
  end
  step = 1;
  x = x0;
  while(x+h <= xn)
    sol = vertcat(sol,zeros(1,n+1));
    copy_y_new = y_new;
    x += h;
    step += 1;
    sol(step,1) = x;
    for i=1:n
      p_current = p(i);
      q_current = q(i);
      for j=1:n
        y_current_str = strcat('y',num2str(j));
        p_current = subs(p_current,y_current_str,sol(step-1,j+1));
        q_current = subs(q_current,y_current_str,sol(step-1,j+1));
      end
      % p_current = double(p_current)
      % q_current = double(q_current)
    end
    for i=2:n+1
        y_current_str = strcat('y',num2str(i-1));
        y_new(i-1) = subs(y_new(i-1),'x',x);
        for j=1:n
          y_new(j) = subs(y_new(j),y_current_str,sol(step-1,i));
        end
    end
    for i=1:n
      sol(step,i+1) = double(y_new(i));
    end
    y_new = copy_y_new;
  end
end

function s = get_size(X)
  s = size(X,2);
end

function dy = get_system
  syms x y1;
  n = input("\nEntre com o número de funções do sistema: ");
  dy = [y1];
  for i=2:n
    dy = vertcat(dy,[syms(strcat('y',num2str(i)))]);
  end
  for i=1:n
    dy(i) = [input(strcat("\nEntre com a funçao f",num2str(i)," : "))];
  end
end

function h_x0_xn = get_step_x0_xn
  h_x0_xn = zeros(1,3);
  h_x0_xn(1) = input("\nEntre com passo (h): ");
  h_x0_xn(2) = input('Entre com o valor de x0: ');
  h_x0_xn(3) = input('Entre com o valor de xn: ');
end

function ys = get_initial_values(n)
  ys = zeros(n,1);
  disp("\n");
  for i=1:n
    ys(i) = input(strcat("Entre com o valor inicial de y",num2str(i)," : "));
  end
end

dy = get_system;
ys = get_initial_values(get_size(dy));
h_x0_xn = get_step_x0_xn;
sol = ode_system(dy,ys,h_x0_xn);
sol
  % disp("\nOs valores são: "),disp(horzcat(xi, yi));
