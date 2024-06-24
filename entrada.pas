

program AnalisadorLexicoTeste;
var
  numero: integer;
  texto: string;
begin
  numero := 10;
  texto := 'Olá, mundo!';
  writeln("O número é: ", numero);
  if numero > 5 then
    writeln("O número é maior que 5.")
  else
    writeln('O número é menor ou igual a 5.');
  for numero := 1 to 5 do
    writeln('Número atual: ', numero);
  repeat
    writeln('Loop infinito');
  until false;
end.
