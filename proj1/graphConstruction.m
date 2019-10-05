clear;clc;
warning('off', 'all');
file = fileread("graphInput.txt");       % collect file contents into string
lines = strsplit(file, "\n")';
numbers=[];
for i=2:rows(lines)
  numbers=[numbers;str2num(lines{i,:})];  %convert string lines into numbers
endfor
k=unique(numbers(:,1));
lgnd=[];
for i=1:rows(k)
  idx=numbers(:,1)==k(i);
  alpha=numbers(idx,2);
  entropy=numbers(idx,3);
  plot(alpha, entropy);
  lgnd=[lgnd;["k equals to ", int2str(k(i))]];
  hold on;
endfor
legend(lgnd);
xlabel("alpha");
ylabel("entropy");  