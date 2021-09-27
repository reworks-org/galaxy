require("turtle")
local complex   = require("complex")
local chartmap  = require("chartmap")
local colormap  = require("colormap")
local common    = require("common")
local logStatus = common.logStatus

io.stdout:setvbuf("no")

local tPar = {}
local function makeTastCase(fCompl)
  for ID = 1, #tPar do
    local test = tPar[ID]
    logStatus("Status: "..test.Name)
    for IR = 1, #test do
      local row = test[IR]
      local fun = (row.Foo or fCompl)
      local num = tostring(fun(unpack(row.Arg)))
      if(num ~= row.Out) then
        logStatus("  FAIL: ("..row.Typ..") <"..num.."> = <"..row.Out..">")
        error("Complex test #"..ID.." found mistmatch at index #"..IR)
      else local sM = (row.Msg and (": "..tostring(row.Msg)) or "")
        logStatus("  OK: ("..row.Typ..") <"..num..">"..sM)
      end
    end
  end; common.tableClear(tPar); collectgarbage()
end

local function testTranslate(na, nb)
  return common.convSignString(na)..common.convSignString(nb).."i"
end

local function testCreateRI(tA)
  return table.unpack(tA)
end

local function testCreateCO(tA)
  return complex.getNew(table.unpack(tA))
end

local function testCreateNA(tA)
  return "test"..{}
end

logStatus("\nThis file is the complex library self test. It will generate error on mismatch.")
logStatus("Methods starting with upper letter make internal changes and return `self`.")
logStatus("Methods starting with lower case return something and do not change internals.")

logStatus("\nCreating complex")
local a = complex.getNew(7,7):Println("%2d"," Create:           {","}")
complex.getNew(a):Println("%2d"," Copy-constructor: {","}")

--------------------------------------------------------------------------
logStatus("\nCalling the copy-constructor for different arguments "..tostring(a)); tPar = {}
logStatus("The functionality manages the values internally and converts to number !")
logStatus("If the conversion to a number cannot be completed a default internal value is used !")

tPar[1] = {
  Name = "Complex copy-constructor method",
  {Typ="nil-value", Arg={a}, Out="{7,7}"},
  {Typ="complex", Arg={complex.getNew(1,-1)}, Out="{1,-1}"},
  {Typ="nil-number1", Arg={a,-7,-7}, Out="{-7,-7}"},
  {Typ="nil-number2", Arg={a,-7,nil}, Out="{-7,0}"},
  {Typ="nil-number3", Arg={a,nil,-7}, Out="{0,-7}"},
  {Typ="nil-number4", Arg={a,nil,nil}, Out="{7,7}", Msg="Copy-constructor method is going to return SELF!"},
  {Typ="string1", Arg={a,"7","7"}, Out="{7,7}"},
  {Typ="string2", Arg={a,"","7"}, Out="{0,7}"},
  {Typ="string3", Arg={a,"7",""}, Out="{7,0}"},
  {Typ="string4", Arg={a,"",""}, Out="{0,0}"},
  {Typ="string5", Arg={a,"abc","def"}, Out="{0,0}"},
  {Typ="bool1", Arg={a,true ,true }, Out="{0,0}", Msg="Copy-constructor method is designed for complex and numbers!"},
  {Typ="bool2", Arg={a,false,false}, Out="{7,7}", Msg="Copy-constructor method is designed for complex and numbers!"},
  {Typ="bool3", Arg={a,true ,false}, Out="{0,0}", Msg="Copy-constructor method is designed for complex and numbers!"},
  {Typ="bool4", Arg={a,false,true }, Out="{0,0}", Msg="Copy-constructor method is designed for complex and numbers!"}
}

makeTastCase(a.getNew)

tPar[1] = {
  Name = "Complex copy-constructor",
  {Typ="nil-value", Arg={a}, Out="{7,7}"},
  {Typ="complex", Arg={complex.getNew(1,-1)}, Out="{1,-1}"},
  {Typ="nil-number1", Arg={-7,-7}, Out="{-7,-7}"},
  {Typ="nil-number2", Arg={-7,nil}, Out="{-7,0}"},
  {Typ="nil-number3", Arg={nil,-7}, Out="{0,-7}"},
  {Typ="nil-number4", Arg={nil,nil}, Out="{0,0}", Msg="Copy-constructor converts arguments to numbers!"},
  {Typ="string1", Arg={"7","7"}, Out="{7,7}"},
  {Typ="string2", Arg={"","7"}, Out="{0,7}"},
  {Typ="string3", Arg={"7",""}, Out="{7,0}"},
  {Typ="string4", Arg={"",""}, Out="{0,0}"},
  {Typ="string5", Arg={"abc","def"}, Out="{0,0}"},
  {Typ="function1", Arg={testCreateRI,{7,7}}, Out="{7,7}"},
  {Typ="function2", Arg={testCreateCO,{7,7}}, Out="{7,7}"},
  {Typ="functionE", Arg={testCreateNA,{7,7}}, Out="nil"},
  {Typ="bool1", Arg={true ,true }, Out="{1,1}", Msg="Copy-constructor supports booleans!"},
  {Typ="bool2", Arg={false,false}, Out="{0,0}", Msg="Copy-constructor supports booleans!"},
  {Typ="bool3", Arg={true ,false}, Out="{1,0}", Msg="Copy-constructor supports booleans!"},
  {Typ="bool4", Arg={false,true }, Out="{0,1}", Msg="Copy-constructor supports booleans!"}
}

makeTastCase(complex.getNew)

--------------------------------------------------------------------------
logStatus("\nConverting complex from something else "..tostring(a))
logStatus("The type of the first argument is used to identify what is converted !"); tPar = {}

tPar[1] = {
  Name = "Directly using a complex with copy-constructor",
  {Typ="copy-constructor", Arg={a}, Out="{7,7}"}
}

tPar[2] = {
  Name = "Translator function",
  {Typ="function result", Arg={testTranslate,7,7}, Out="{7,7}"}
}

tPar[3] = {
  Name = "Converting tables to a complex number. Convert form a table with given keys",
  {Typ="table number keys 1,2", Arg={{a:getReal(),a.getImag()}},Out="{7,7}"},
  {Typ="table predefined keys1", Arg={{["Real"] = a:getReal(), ["Imag"]=a.getImag()}},Out="{7,7}"},
  {Typ="table predefined keys2", Arg={{["real"] = a:getReal(), ["imag"]=a.getImag()}},Out="{7,7}"},
  {Typ="table predefined keys3", Arg={{["Re"] = a:getReal(), ["Im"]=a.getImag()}},Out="{7,7}"},
  {Typ="table predefined keys4", Arg={{["re"] = a:getReal(), ["im"]=a.getImag()}},Out="{7,7}"},
  {Typ="table predefined keys5", Arg={{["R"] = a:getReal(), ["I"]=a.getImag()}},Out="{7,7}"},
  {Typ="table predefined keys6", Arg={{["r"] = a:getReal(), ["i"]=a.getImag()}},Out="{7,7}"},
  {Typ="table predefined keys7", Arg={{["X"] = a:getReal(), ["Y"]=a.getImag()}},Out="{7,7}"},
  {Typ="table predefined keys8", Arg={{["x"] = a:getReal(), ["y"]=a.getImag()}},Out="{7,7}"},
  {Typ="table custom key data", Arg={{["asd"] = a:getReal(), ["fgh"]=a.getImag()},"asd","fgh"},Out="{7,7}"},
}

tPar[4] = {
  Name = "String with variety of outputs",
  {Typ="string +i", Arg={"7+i7"},Out="{7,7}"},
  {Typ="string i+", Arg={"7+7i"},Out="{7,7}"},
  {Typ="string +I", Arg={"7+I7"},Out="{7,7}"},
  {Typ="string I+", Arg={"7+7I"},Out="{7,7}"},
  {Typ="string +j", Arg={"7+j7"},Out="{7,7}"},
  {Typ="string j+", Arg={"7+7j"},Out="{7,7}"},
  {Typ="string +J", Arg={"7+J7"},Out="{7,7}"},
  {Typ="string J+", Arg={"7+7J"},Out="{7,7}"},
  {Typ="string +k", Arg={"7+k7"},Out="{7,7}"},
  {Typ="string k+", Arg={"7+7k"},Out="{7,7}"},
  {Typ="string +K", Arg={"7+K7"},Out="{7,7}"},
  {Typ="string K+", Arg={"7+7K"},Out="{7,7}"}
}

tPar[5] = {
  Name = "String with variety of key storage",
  {Typ="default format", Arg={"7,7"},Out="{7,7}"},
  {Typ="default format", Arg={"7,-7"},Out="{7,-7}"},
  {Typ="default format", Arg={"-7,7"},Out="{-7,7}"},
  {Typ="default format", Arg={"-7,-7"},Out="{-7,-7}"},
  {Typ="string custom delimiter &", Arg={"7&7","&"},Out="{7,7}"},
  {Typ="string complex format {}", Arg={"{7,7}"  },Out="{7,7}"},
  {Typ="string complex format //", Arg={"/7,7/"  },Out="{7,7}"},
  {Typ="string complex format []", Arg={"[7,7]"  },Out="{7,7}"},
  {Typ="string complex format ()", Arg={"(7,7)"  },Out="{7,7}"},
  {Typ="string complex format ||", Arg={"|7,7|"  },Out="{7,7}"},
  {Typ="string complex format <>", Arg={"<7,7>"  },Out="{7,7}"},
  {Typ="string complex format /{}/", Arg={"/{7,7}/"},Out="{7,7}"},
  {Typ="string direct 1", Arg={"+i"},Out="{0,1}"},
  {Typ="string direct 2", Arg={"i" },Out="{0,1}"},
  {Typ="string direct 3", Arg={"-i"},Out="{0,-1}"},
}

tPar[6] = {
  Name = "Numbers",
  {Typ="number", Arg={7,7},Out="{7,7}"},
  {Typ="number", Arg={-7,7},Out="{-7,7}"},
  {Typ="number", Arg={7,-7},Out="{7,-7}"},
  {Typ="number", Arg={-7,-7},Out="{-7,-7}"}
}

tPar[7] = {
  Name = "Boolean and non-existent",
  {Typ="boolean", Arg={true, true  },Out="{1,1}"},
  {Typ="boolean", Arg={true, false },Out="{1,0}"},
  {Typ="boolean", Arg={true,  nil  },Out="{1,0}"},
  {Typ="boolean", Arg={false, true },Out="{0,1}"},
  {Typ="boolean", Arg={false, false},Out="{0,0}"},
  {Typ="boolean", Arg={false, nil  },Out="{0,0}"},
  {Typ="non-existent", Arg={nil  , nil  },Out="{0,0}"}
}

makeTastCase(complex.convNew)

-------------------------------------------------------------------------------------------------------
logStatus("\nComplex signum "..tostring(a)); tPar = {}

tPar[1] = {
  Name = "Test different flags for chosing the corrct sign function",
  {Typ="element-wise", Arg={a, true, false, false}, Out=    "1"},
  {Typ="c-sign"      , Arg={a, false, true, false}, Out="{1,1}"},
  {Typ="n-sign"      , Arg={a, false, false, true}, Out="{1,1}"},
  {Typ="unit-sign"   , Arg={a}, Out="{0.70710678118655,0.70710678118655}"}
}

makeTastCase(a.getSign)
-------------------------------------------------------------------------------------------------------
logStatus("\nConverting to string. Variety of outputs "..tostring(a)); tPar = {}

logStatus("Export n/a 1: "..tostring(a))
tPar[1] = {
  Name = "Simple export",
  {Typ="Export n/s 2", Arg={a}, Out="{7,7}"}
}

tPar[2] = {
  Name = "Table export",
  {Typ="Export table 1", Arg={a,"table"}, Out="{[1]=7.000000,[2]=7.000000}"},
  {Typ="Export table 2", Arg={a,"table",1,"%f",1}, Out="{[1]=7.000000,[2]=7.000000}"},
  {Typ="Export table 3", Arg={a,"table",3,"%5.2f",4}, Out="<[\"real\"]= 7.00,[\"imag\"]= 7.00>"},
  {Typ="Export table 4", Arg={a,"table",3,"%5.2f",4,"asd","fgh"}, Out="<[\"asd\"]= 7.00,[\"fgh\"]= 7.00>"}
}

tPar[3] = {
  Name = "String export",
  {Typ="Export string 1", Arg={a,"string"}           , Out="7+7i"},
  {Typ="Export string 2", Arg={a,"string",1}         , Out="7+7i"},
  {Typ="Export string 3", Arg={a,"string",1,false}   , Out="7+7i"},
  {Typ="Export string 4", Arg={a,"string",1,true}    , Out="7+i7"},
  {Typ="Export string 5", Arg={a,"string",2,true}    , Out="7+I7"},
  {Typ="Export string 6", Arg={a,"string",3,true}    , Out="7+j7"},
  {Typ="Export string 7", Arg={a,"string",4,true}    , Out="7+J7"},
  {Typ="Export string 8", Arg={a,"string",4,true,"$"}, Out="7+$7"}
}

makeTastCase(a.getFormat)

logStatus("\nConverting to polar coordinates and back "..tostring(a))
local r, p = a:getPolar(); logStatus("1: "..r.."e^"..p.."i")
local b = complex.getNew():Euler(r, p):Print(nil,"2: {","}\n")
local sa, sb = tostring(a), tostring(b)
if(sa ~= sb) then error("Euler conversion error <"..sa.."> = <"..sb..">!") end

logStatus("\nAngle handling radian to degree and back "..tostring(a))
local r, d = a:getAngRad(), a:getAngDeg()
logStatus("1: Angle in radian "..r.." is "..complex.toDeg(r).." in degrees")
logStatus("2: Angle in degree "..d.." is "..complex.toRad(d).." in radians")

--------------------------------------------------------
logStatus("\nClass methods test"..tostring(a)); tPar= {}

tPar[1] ={
  Name = "Addition "..tostring(a),
  {Typ="Add 1" , Arg={a, 1}     ,Foo=a.__add , Out="{8,7}"},
  {Typ="Add 2" , Arg={a, 2}     ,Foo=a.__add , Out="{9,7}"},
  {Typ="Add 3" , Arg={a, 2}     ,Foo=a.getAdd, Out="{9,7}"},
  {Typ="Add F1", Arg={a, .7, .7},Foo=a.getAdd, Out="{7.7,7.7}"},
  {Typ="Add F2", Arg={a, a/10}  ,Foo=a.getAdd, Out="{7.7,7.7}"}
}

tPar[2] ={
  Name = "Subtraction "..tostring(a),
  {Typ="Sub 1" , Arg={a, 1}     ,Foo=a.__sub , Out="{6,7}"},
  {Typ="Sub 2" , Arg={a, 2}     ,Foo=a.__sub , Out="{5,7}"},
  {Typ="Sub 3" , Arg={a, 2}     ,Foo=a.getSub, Out="{5,7}"},
  {Typ="Sub F1", Arg={a, .7, .7},Foo=a.getSub, Out="{6.3,6.3}"},
  {Typ="Sub F2", Arg={a, a/10}  ,Foo=a.getSub, Out="{6.3,6.3}"}
}

tPar[3] ={
  Name = "Multiplication "..tostring(a),
  {Typ="Mul 1" , Arg={a, 1}     ,Foo=a.__mul , Out="{7,7}"},
  {Typ="Mul 2" , Arg={a, 2}     ,Foo=a.__mul , Out="{14,14}"},
  {Typ="Mul 3" , Arg={a, 2}     ,Foo=a.getMul, Out="{14,14}"},
  {Typ="Mul F1", Arg={a, .7, .7},Foo=a.getMul, Out="{0,9.8}"},
  {Typ="Mul F2", Arg={a, a/10}  ,Foo=a.getMul, Out="{0,9.8}"},
  {Typ="Mul UE", Arg={a, 2, 2, true}  ,Foo=a.getMul, Out="{14,14}"}
}

tPar[4] ={
  Name = "Division "..tostring(a),
  {Typ="Div 1" , Arg={a, 1}     ,Foo=a.__div , Out="{7,7}"},
  {Typ="Div 2" , Arg={a, 2}     ,Foo=a.__div , Out="{3.5,3.5}"},
  {Typ="Div 3" , Arg={a, 2}     ,Foo=a.getDiv, Out="{3.5,3.5}"},
  {Typ="Div F1", Arg={a, .7, .7},Foo=a.getDiv, Out="{10,0}"},
  {Typ="Div F2", Arg={a, a/10}  ,Foo=a.getDiv, Out="{10,0}"},
  {Typ="Div UE", Arg={a, 2, 2, true}  ,Foo=a.getDiv, Out="{3.5,3.5}"}
}

tPar[5] ={
  Name = "Power "..tostring(a),
  {Typ="Pow 1" , Arg={a, 1}     ,Foo=a.__pow , Out="{7,7}"},
  {Typ="Pow 2" , Arg={a, 2}     ,Foo=a.__pow , Out="{6.0005711337296e-015,98}"},
  {Typ="Pow 3" , Arg={a, 2}     ,Foo=a.getPow, Out="{6.0005711337296e-015,98}"},
  {Typ="Pow F1", Arg={a, .7, .7},Foo=a.getPow, Out="{-1.5827757183203,2.3963307116848}"},
  {Typ="Pow F2", Arg={a, a/10}  ,Foo=a.getPow, Out="{-1.5827757183203,2.3963307116848}"},
  {Typ="Pow UE", Arg={a, 2, 2, true}  ,Foo=a.getPow, Out="{49,49}"}
}

local t = a:getNew():Add(complex.convNew("0.5+0.5i"))

tPar[6] ={
  Name = "Floor "..tostring(a),
  {Typ="Nude"   , Arg={t}              ,Foo=t.getFloor, Out="{7,7}"},
  {Typ="ApplyFF", Arg={t, false, false},Foo=t.getFloor, Out="{7.5,7.5}"},
  {Typ="ApplyFT", Arg={t, false, true },Foo=t.getFloor, Out="{7.5,7}"},
  {Typ="ApplyTF", Arg={t, true, false },Foo=t.getFloor, Out="{7,7.5}"},
  {Typ="ApplyTT", Arg={t, true,  true },Foo=t.getFloor, Out="{7,7}"}
}

tPar[7] ={
  Name = "Ceil "..tostring(a),
  {Typ="Nude"   , Arg={t}              ,Foo=t.getCeil, Out="{8,8}"},
  {Typ="ApplyFF", Arg={t, false, false},Foo=t.getCeil, Out="{7.5,7.5}"},
  {Typ="ApplyFT", Arg={t, false, true },Foo=t.getCeil, Out="{7.5,8}"},
  {Typ="ApplyTF", Arg={t, true, false },Foo=t.getCeil, Out="{8,7.5}"},
  {Typ="ApplyTT", Arg={t, true,  true },Foo=t.getCeil, Out="{8,8}"}
}

tPar[8] ={
  Name = "Negate "..tostring(a),
  {Typ="Neg"  , Arg={a}              ,Foo=t.getNeg  , Out="{-7,-7}"},
  {Typ="NegRe", Arg={a, false, false},Foo=t.getNegRe, Out="{-7,7}"},
  {Typ="NegIm", Arg={a, false, true },Foo=t.getNegIm, Out="{7,-7}"},
  {Typ="Conj" , Arg={a, true, false },Foo=t.getConj , Out="{7,-7}"},
}

local ru = ( a:getNew() * 10 + complex.convNew(" 0.36+0.36j")):Print("%7.3f","Positive : {","}\n")
local rd = (-a:getNew() * 10 + complex.convNew("-0.36-0.36j")):Print("%7.3f","Negative : {","}\n")

tPar[9] ={
  Name = "Round positive "..tostring(a),
  {Typ="Nude"                          , Arg={nil}    ,Foo=t.getRound, Out="{0,0}"},
  {Typ="Zero"                          , Arg={0}      ,Foo=t.getRound, Out="{0,0}"},
  {Typ="Round away positive integer"   , Arg={t,  1  },Foo=t.getRound, Out="{70,70}"},
  {Typ="Round away positive float"     , Arg={t,  0.1},Foo=t.getRound, Out="{70.4,70.4}"},
  {Typ="Round towards positive integer", Arg={t, -1  },Foo=t.getRound, Out="{70,70}"},
  {Typ="Round towards positive float"  , Arg={t, -0.1},Foo=t.getRound, Out="{70.3,70.3}"}
}

tPar[9] ={
  Name = "Round positive "..tostring(ru),
  -- {Typ="Nil (ERROR)"          , Arg={ru, nil },Foo=t.getRound, Out="{0,0}"},
  {Typ="Zero"                 , Arg={ru,  0  },Foo=t.getRound, Out="{0,0}"},
  {Typ="Round away integer"   , Arg={ru,  1  },Foo=t.getRound, Out="{70,70}"},
  {Typ="Round away float"     , Arg={ru,  0.1},Foo=t.getRound, Out="{70.4,70.4}"},
  {Typ="Round towards integer", Arg={ru, -1  },Foo=t.getRound, Out="{70,70}"},
  {Typ="Round towards float"  , Arg={ru, -0.1},Foo=t.getRound, Out="{70.3,70.3}"}
}

tPar[10] ={
  Name = "Round negative "..tostring(rd),
  -- {Typ="Nil (ERROR)"          , Arg={rd, nil },Foo=t.getRound, Out="{0,0}"},
  {Typ="Zero"                 , Arg={rd,  0  },Foo=t.getRound, Out="{-0,-0}"},
  {Typ="Round away integer"   , Arg={rd,  1  },Foo=t.getRound, Out="{-70,-70}"},
  {Typ="Round away float"     , Arg={rd,  0.1},Foo=t.getRound, Out="{-70.4,-70.4}"},
  {Typ="Round towards integer", Arg={rd, -1  },Foo=t.getRound, Out="{-70,-70}"},
  {Typ="Round towards float"  , Arg={rd, -0.1},Foo=t.getRound, Out="{-70.3,-70.3}"}
}

tPar[11] ={
  Name = "Inf and NaN convert "..tostring(a),
  {Typ="Nan(n,n)", Arg={a}            ,Foo=a.getNan, Out="{7,7}"},
  {Typ="Nan(t,t)", Arg={a,true ,true },Foo=a.getNan, Out="{nan,nan}"},
  {Typ="Nan(f,f)", Arg={a,false,false},Foo=a.getNan, Out="{7,7}"},
  {Typ="Nan(t,f)", Arg={a,true ,false},Foo=a.getNan, Out="{nan,7}"},
  {Typ="Nan(f,t)", Arg={a,false,true },Foo=a.getNan, Out="{7,nan}"},
  {Typ="Inf(n,n)", Arg={a}            ,Foo=a.getInf, Out="{7,7}"},
  {Typ="Inf(t,t)", Arg={a,true ,true },Foo=a.getInf, Out="{-inf,-inf}"},
  {Typ="Inf(f,f)", Arg={a,false,false},Foo=a.getInf, Out="{inf,inf}"},
  {Typ="Inf(t,f)", Arg={a,true ,false},Foo=a.getInf, Out="{-inf,inf}"},
  {Typ="Inf(f,t)", Arg={a,false,true },Foo=a.getInf, Out="{inf,-inf}"}
}

local z = a:getNew(0,0)
local n = a:getNew(1/0, -1/0)
local g = a:getNew(0/0, 0/0)
tPar[12]={
  Name = "Boolean checks "..tostring(z).." > "..tostring(n).." > "..tostring(g),
  {Typ="Zero     ", Arg={z},Foo=z.isZero            , Out="true"},
  {Typ="Zero(f,f)", Arg={z,false,false},Foo=z.isZero, Out="nil"},
  {Typ="Zero(t,t)", Arg={z,true ,true },Foo=z.isZero, Out="true"},
  {Typ="Zero(t,f)", Arg={z,true ,false},Foo=z.isZero, Out="true"},
  {Typ="Zero(f,t)", Arg={z,false,true },Foo=z.isZero, Out="true"},
  {Typ="ZeroRe   ", Arg={z},Foo=z.isZeroReal        , Out="true"},
  {Typ="ZeroIm   ", Arg={z},Foo=z.isZeroImag        , Out="true"},
  {Typ="Inf(xx)  ", Arg={n}             ,Foo=n.isInf, Out="false"},
  {Typ="Inf(++)  ", Arg={n,false, false},Foo=n.isInf, Out="false"},
  {Typ="Inf(--)  ", Arg={n,true , true },Foo=n.isInf, Out="false"},
  {Typ="Inf(+-)  ", Arg={n,false, true },Foo=n.isInf, Out="true"},
  {Typ="Inf(-+)  ", Arg={n,true , false},Foo=n.isInf, Out="false"},
  {Typ="InfRe    ", Arg={n},Foo=n.isInfReal         , Out="true"},
  {Typ="InfRe    ", Arg={n,true} ,Foo=n.isInfReal   , Out="false"},
  {Typ="InfRe    ", Arg={n,false},Foo=n.isInfReal   , Out="true"},
  {Typ="InfIm    ", Arg={n,true} ,Foo=n.isInfImag   , Out="true"},
  {Typ="InfIm    ", Arg={n,false},Foo=n.isInfImag   , Out="false"},
  {Typ="Nan      ", Arg={g},Foo=g.isNan             , Out="true"},
  {Typ="NanRe    ", Arg={g},Foo=g.isNanReal         , Out="true"},
  {Typ="NanIm    ", Arg={g},Foo=g.isNanImag         , Out="true"}
}

local z = a:getNew(2,2)
local n = a:getNew(2,2)
tPar[13] ={
  Name = "Exponential "..tostring(z),
  {Typ="NoArg" , Arg={z,}        ,Foo=z.getExp, Out="{-3.0749323206394,6.7188496974282}"},
  {Typ="Cmpl"  , Arg={z,n}       ,Foo=z.getExp, Out="{-3.0749323206394,6.7188496974282}"},
  {Typ="ImRe"  , Arg={z,2,2}     ,Foo=z.getExp, Out="{-3.0749323206394,6.7188496974282}"},
  {Typ="ImReS" , Arg={z,2,"2"}   ,Foo=z.getExp, Out="{-3.0749323206394,6.7188496974282}"},
  {Typ="ImSRe" , Arg={z,"2",2}   ,Foo=z.getExp, Out="{-3.0749323206394,6.7188496974282}"},
  {Typ="ImSReS", Arg={z,"2","2"} ,Foo=z.getExp, Out="{-3.0749323206394,6.7188496974282}"},
  {Typ="BCmpl"  , Arg={z,n,true}       ,Foo=z.getExp, Out="{7.3890560989306,0}"},
  {Typ="BImRe"  , Arg={z,2,2,true}     ,Foo=z.getExp, Out="{7.3890560989306,0}"},
  {Typ="BImReS" , Arg={z,2,"2",true}   ,Foo=z.getExp, Out="{7.3890560989306,0}"},
  {Typ="BImSRe" , Arg={z,"2",2,true}   ,Foo=z.getExp, Out="{7.3890560989306,0}"},
  {Typ="BImSReS", Arg={z,"2","2",true} ,Foo=z.getExp, Out="{7.3890560989306,0}"}
}

tPar[14] ={
  Name = "Dot product "..tostring(z),
  {Typ="NoArg" , Arg={z,}        ,Foo=z.getDot, Out="0"},
  {Typ="Number", Arg={z,2}       ,Foo=z.getDot, Out="4"},
  {Typ="Cmpl"  , Arg={z,n}       ,Foo=z.getDot, Out="8"},
  {Typ="ImRe"  , Arg={z,2,2}     ,Foo=z.getDot, Out="8"},
  {Typ="ImReS" , Arg={z,2,"2"}   ,Foo=z.getDot, Out="8"},
  {Typ="ImSRe" , Arg={z,"2",2}   ,Foo=z.getDot, Out="8"},
  {Typ="ImSReS", Arg={z,"2","2"} ,Foo=z.getDot, Out="8"},
}

tPar[15] ={
  Name = "Cross product "..tostring(z),
  {Typ="NoArg" , Arg={z,}        ,Foo=z.getCross, Out="0"},
  {Typ="Number", Arg={z,2}       ,Foo=z.getCross, Out="-4"},
  {Typ="Cmpl"  , Arg={z,n}       ,Foo=z.getCross, Out="0"},
  {Typ="ImRe"  , Arg={z,2,2}     ,Foo=z.getCross, Out="0"},
  {Typ="ImReS" , Arg={z,2,"2"}   ,Foo=z.getCross, Out="0"},
  {Typ="ImSRe" , Arg={z,"2",2}   ,Foo=z.getCross, Out="0"},
  {Typ="ImSReS", Arg={z,"2","2"} ,Foo=z.getCross, Out="0"},
}

tPar[16] ={
  Name = "Mid point "..tostring(z),
  {Typ="NoArg" , Arg={z,}        ,Foo=z.getMid, Out="{1,1}"},
  {Typ="Number", Arg={z,2}       ,Foo=z.getMid, Out="{2,1}"},
  {Typ="Cmpl"  , Arg={z,n}       ,Foo=z.getMid, Out="{2,2}"},
  {Typ="ImRe"  , Arg={z,2,2}     ,Foo=z.getMid, Out="{2,2}"},
  {Typ="ImReS" , Arg={z,2,"2"}   ,Foo=z.getMid, Out="{2,2}"},
  {Typ="ImSRe" , Arg={z,"2",2}   ,Foo=z.getMid, Out="{2,2}"},
  {Typ="ImSReS", Arg={z,"2","2"} ,Foo=z.getMid, Out="{2,2}"},
}

local z = a:getNew(-2,2)
tPar[17] ={
  Name = "Sign "..tostring(z),
  {Typ="TTT", Arg={z,true ,true ,true },Foo=z.getSign, Out="-1"},
  {Typ="TTF", Arg={z,true ,true ,false},Foo=z.getSign, Out="-1"},
  {Typ="TFT", Arg={z,true ,false,true },Foo=z.getSign, Out="-1"},
  {Typ="TFF", Arg={z,true ,false,false},Foo=z.getSign, Out="-1"},
  {Typ="FTT", Arg={z,false,true ,true },Foo=z.getSign, Out="{-1,1}"},
  {Typ="FTF", Arg={z,false,true ,false},Foo=z.getSign, Out="{-1,1}"},
  {Typ="FFT", Arg={z,false,false,true },Foo=z.getSign, Out="{-1,1}"},
  {Typ="NNN", Arg={z,}                 ,Foo=z.getSign, Out="{-0.70710678118655,0.70710678118655}"},
  {Typ="FFF", Arg={z,false,false,false},Foo=z.getSign, Out="{-0.70710678118655,0.70710678118655}"},  
}

local z = a:getNew(5,math.pi/2)
tPar[18] ={
  Name = "Euler presentation "..tostring(z),
  {Typ="NoArg" , Arg={z,}        ,Foo=z.getEuler, Out="{3.0615158845559e-016,5}"},
  {Typ="Number", Arg={z,2}       ,Foo=z.getEuler, Out="{2,0}"},
  {Typ="Cmpl"  , Arg={z,n}       ,Foo=z.getEuler, Out="{-0.83229367309428,1.8185948536514}"},
  {Typ="ImRe"  , Arg={z,2,2}     ,Foo=z.getEuler, Out="{-0.83229367309428,1.8185948536514}"},
  {Typ="ImReS" , Arg={z,2,"2"}   ,Foo=z.getEuler, Out="{-0.83229367309428,1.8185948536514}"},
  {Typ="ImSRe" , Arg={z,"2",2}   ,Foo=z.getEuler, Out="{-0.83229367309428,1.8185948536514}"},
  {Typ="ImSReS", Arg={z,"2","2"} ,Foo=z.getEuler, Out="{-0.83229367309428,1.8185948536514}"},
}

local v1 = complex.getNew(5,0)
local v2 = complex.getNew(-2,0)
local M = {10, "25"}
tPar[19] = {
  Name = "Center of mass ("..table.concat({tostring(v1),M[1],tostring(v2),M[2]},",")..")",
  {Typ="CMCM", Arg={v1,v1,  M[1] ,  v2, M[2]},Foo=v1.getCenterMass, Out="{0,0}"},
  {Typ="NMNM", Arg={v1, 5,  M[1] ,  -2, M[2]},Foo=v1.getCenterMass, Out="{0,0}"},
  {Typ="SMSM", Arg={v1,"5", M[1] ,"-2", M[2]},Foo=v1.getCenterMass, Out="{0,0}"},
  {Typ="CTT" , Arg={v1,{v1, v2}  ,      M   },Foo=v1.getCenterMass, Out="{0,0}"},
  {Typ="NTT" , Arg={v1,{5, -2}   ,      M   },Foo=v1.getCenterMass, Out="{0,0}"},
  {Typ="STT" , Arg={v1,{"5","-2"},      M   },Foo=v1.getCenterMass, Out="{0,0}"},
}

makeTastCase()
--------------------------------------------------------

logStatus("\nComparison operators")
logStatus("Compare less            : "..tostring(complex.getNew(1,2) <  complex.getNew(2,3)))
logStatus("Compare less or equal   : "..tostring(complex.getNew(1,2) <= complex.getNew(2,3)))
logStatus("Compare grater          : "..tostring(complex.getNew(5,6) >  complex.getNew(2,3)))
logStatus("Compare greater or equal: "..tostring(complex.getNew(2,4) >= complex.getNew(2,3)))
logStatus("Compare equal           : "..tostring(complex.getNew(1,2) == complex.getNew(1,2)))

logStatus("\nComplex number call using the \"__call\" method "..tostring(a))
--[[
  When calling the complex number as a function
  the first argument is the method you want to call given as string,
  the next are var-args, which are the parameters of the method.
  The call will always return a status and varargs representing the result of the call.
  For example let's set the complex number's real and imaginery parts via complex call.
]]
local bSuccess, cNum = a:getNew()("Set",1,1)
if(bSuccess) then
  logStatus("The complex call was successful. The result is "..tostring(cNum).."\n")
else
  logStatus("The complex call was not successful. Error: "..cNum.."\n")
end

local tCall = {
  {"MirrorPoint     : ","getMirrorPoint","{-7,-1}                               "},
  {"Margin          : ","getMargin     ","{7,1}                                 "},
  {"Reverse         : ","getRev        ","{0.14,-0.02}                          "},
  {"Unit            : ","getUnit       ","{0.98994949366117,0.14142135623731}   "},
  {"PythagorAdd     : ","getAddPyth    ","{9.8994949366117,1.4142135623731}     "},
  {"Nudge           : ","getNudge      ","{7.000000000099,1.0000000000141}      "},
  {"EulerConversion : ","getEuler      ","{3.782116141077,5.8902968936553}      "},
  {"Sine            : ","getSin        ","{1.0137832978161,0.885986829195}      "},
  {"Cosine          : ","getCos        ","{1.1633319692207,-0.7720914350223}    "},
  {"Tangent         : ","getTang       ","{0.25407140331504,0.93021872707887}   "},
  {"Cotangent       : ","getCotg       ","{0.27323643702064,-1.0003866917748}   "},
  {"Hyp sine        : ","getSinH       ","{296.25646574921,461.392875559}       "},
  {"Hyp cosine      : ","getCosH       ","{296.25695844114,461.39210823679}     "},
  {"Hyp tangent     : ","getTangH      ","{1.0000006920752,1.5122148957712e-006}"},
  {"Hyp cotangent   : ","getCotgH      ","{0.999999307923,-1.5122128026371e-006}"},
  {"Logarithm       : ","getLog        ","{1.9560115027141,0.14189705460416}    "},
  {"Exponential     : ","getExp        ","{592.51342419036,922.78498379579}     "},
  {"ArgSine         : ","getArcSin     ","{1.4274787937403,2.6443267863946}     "},
  {"ArgCosine       : ","getArcCos     ","{0.14331753305457,-2.6443267863946}   "},
  {"ArgTangent      : ","getArcTang    ","{1.4316464972923,0.019617903860374}   "},
  {"ArgCotangent    : ","getArcCotg    ","{0.13914982950256,-0.019617903860374} "},
  {"ArgSineH        : ","getArcSinH    ","{2.6539273355384,0.14051690607218}    "},
  {"ArgCosineH      : ","getArcCosH    ","{2.6443267863946,0.14331753305457}    "},
  {"ArgTangentH     : ","getArcTangH   ","{0.14086733931285,1.550399485361}     "},
  {"ArgCotangentH   : ","getArcCotgH   ","{0.14086733931285,-0.020396841433933} "}
}

local b = complex.getNew(7,1)
for id = 1, #tCall do
  local mth = common.stringTrim(tCall[id][2])
  local suc, rez = b(mth)
  if(suc) then rez = tostring(rez)
    local nam = tCall[id][1]:gsub(":", ""); nam = common.stringTrim(nam)
    local com = common.stringTrim(tCall[id][3])
    local sta = ((rez == com) and "OK" or "FAIL")
    if(sta == "OK") then logStatus(common.stringPadR(nam,20," ")..sta.." >> "..rez)
    else error("There was a problem executing method <"..nam.."> at index #"..id.."<"..rez.."="..com..">") end
  else local nam = tCall[id][1]:gsub(":","")
    error("There was a problem executing method <"..nam.."> at index #"..id.."<"..rez.."="..com..">")
  end
end; logStatus("")

logStatus("Complex roots returns a table of complex numbers being the roots of the base number "..tostring(a))

local W, H   = 800, 800 -- Window size
local dX, dY = 1, 1     -- Coordinate system step
local gAlp   = 200      -- Coordinate system grey alpha level
local R      = 5        -- Roots base

local r = a:getRoots(R)

if(r) then
  
  -- Allocate colours
  local clGrn = colr(colormap.getColorGreenRGB())
  local clRed = colr(colormap.getColorRedRGB())
  local clBlk = colr(colormap.getColorBlackRGB())
  local clGry = colr(colormap.getColorPadRGB(gAlp))
  
  -- Adjust the mapping intervals according to the number rooted
  local nN = r[1]:getNorm() * 1.25
  local intX = chartmap.New("interval","WinX", -nN, nN, 0, W)
  local intY = chartmap.New("interval","WinY", -nN, nN, H, 0)
  
  --[[
    Custom function for drawing a number on the complex plane
    The the first argument must always be the complex number
    that you are gonna draw a.k.a. SELF. The other parameters are
    VARARG, which means you can use a bunch of them in the stack.
    In this example all the arguments are local for this file
    so there is no point of extending the vararg on the stack
    Prototype: drawFunction(SELF, ...)
  ]]
  local function drawComplexFunction(C)
    local r = C:getRound(0.001)
    local x = intX:Convert(r:getReal()):getValue()
    local y = intY:Convert(r:getImag()):getValue()
    local ox = intX:Convert(0):getValue()
    local oy = intY:Convert(0):getValue()
    pncl(clGrn); line(ox, oy, x, y)
    pncl(clRed); rect(x-2,y-2,5,5)
    pncl(clBlk); text(tostring(r),r:getAngDeg()+90,x,y)
  end
  
  --[[
    It is easy for the complex numbers to be drawn using a attached drawing method
    internally as every object keeps its local data and you can draw stuff using less arguments
    Prototype: complex.setAction(KEY, FUNCTION)
  ]]
  complex.setAction("This your action key !" ,drawComplexFunction) -- This is how you register a drawing method
    
  open("Graphical complex roots for "..tostring(a))
  size(W, H); zero(0, 0); updt(false) -- disable auto updates
  
  local scOpe = chartmap.New("scope"):setInterval(intX, intY):setSize():setBorder()
        scOpe:setColor():setDelta(dX, dY):Draw(true, true, true)
  
  for id = 1, #r do
    local ppw = (r[id]^R); ppw:Round(0.0000000001)
    logStatus(common.stringPadR(r[id].."^"..R, 38, " ").." = "..ppw)
    spw, sa = tostring(ppw), tostring(a); if(spw ~= sa) then
      error("Complex power mismatch at #"..id.." <"..spw..">?=<"..sa..">") end
    r[id]:Action("This your action key !")
    -- scOpe:drawComplex(r[id], nil, true) -- This is the same as above
    updt(); wait(0.1)
  end
end

wait()
