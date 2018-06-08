print( 'hello, world' )


function f(x)  
    print("function")  
    return x*2   
end  
for i=1,f(5) do print(i)  
end 


days = {"Suanday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"}  
for i,v in ipairs(days) do  print(v) end   


function f2(a,b)
    return a+b
end 

local afunc = f2
function foo (x)
  for i = 1,1000000 do
    x = x + afunc(i,i)
  end
  return x
end
print(foo(10))
print(os.clock())
print("over")


--function fk (k)
--　　return function () return k end
--end
--local lim = 100000
--local a = {}
--for i = 1, lim do
--　　a[i] = fk(i)
--end
--print(a[10]()) --> 10
