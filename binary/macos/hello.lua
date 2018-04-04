print( 'hello, world' )


function f(x)  
    print("function")  
    return x*2   
end  
for i=1,f(5) do print(i)  
end 


days = {"Suanday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"}  
for i,v in ipairs(days) do  print(v) end   



local sin = math.sin
function foo (x)
  for i = 1,1000000 do
    x = x + sin(i)
  end
  return x
end
print(foo(10))
print("over")