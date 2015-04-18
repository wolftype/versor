local V = require"versor"


local C3 = V{
  metric = {1,1,1,1,-1},
  types = {
    { name="Vec", bases={"e1", "e2", "e3"} },
    { name="Biv", bases={"e12", "e13", "e23"} },
    { name="Tri", bases={"e123"} },
    { name="Pss",  bases={"e12345"} },
    { name="Rot",  bases={"s", "e12", "e13", "e23"} },
    { name="Pnt",  bases={"e1", "e2", "e3", "e4", "e5"}, dual=true },
    { name="Dlp",  bases={"e1", "e2", "e3", "e5"}, dual=true },
    { name="Pln",  bases={"e1235", "e1245", "e1345", "e2345"} },
    { name="Sph",  bases={"e1235", "e1234", "e1245", "e1345", "e2345"} },
    { name="Dll",  bases={"e12", "e13", "e23", "e15", "e25", "e35"}, dual=true },
    { name="Lin",  bases={"e145", "e245", "e345", "e125", "e135", "e235"} },
    { name="Flp",  bases={"e15", "e25", "e35", "e45"} },
    { name="Par",  bases={"e12", "e13", "e23", "e14", "e24", "e34", "e15", "e25", "e35", "e45"}, dual=true },
    { name="Cir",  bases={"e123", "e145", "e245", "e345", "e124", "e134", "e234", "e125", "e135", "e235"} },
    { name="Bst",  bases={"s", "e12", "e13", "e23", "e14", "e24", "e34", "e15", "e25", "e35", "e45"} },
    { name="Dil",  bases={"s", "e45"} },
    { name="Mot",  bases={"s", "e12", "e13", "e23", "e15", "e25", "e35", "e1235"} },
    { name="Trs",  bases={"s", "e15", "e25", "e35"} },
    { name="Drv",  bases={"e15", "e25", "e35"} },
    { name="Drb",  bases={"e125", "e135", "e235"} },
    { name="Drt",  bases={"e1235"} },
    { name="Tnv",  bases={"e14", "e24", "e34"} },    
  },
  conformal = true
}

return C3

