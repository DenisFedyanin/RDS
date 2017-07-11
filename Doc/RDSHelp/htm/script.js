 
 
var ClearTimerId;  
var OldColor;  
var ElementId;  
var LightIsOn;  
var LightStartTime;  
 
var LitPictureId;  
var LitPictureOldColor;  
 
 
function ElementToLight(ref)
{ var _h=ref;
  if(_h.length<2)
    return null;
  if(_h[0]!="#")
    { var n=_h.lastIndexOf("#");
      if(n<0)
        return null;
      _h=_h.substr(n);
    }
  if(_h.substr(1,6)=="light_")  
    _h=_h.substr(1);
  else  
    _h="light_"+_h.substr(1);
  return document.getElementById(_h);
}
 
function LightOff()
{ if(!LightIsOn)
    return;
  ElementId.style.backgroundColor=OldColor;
  LightIsOn=false;
  if(ClearTimerId!=null)
    { clearTimeout(ClearTimerId);
      ClearTimerId=null;
    }
}
 
function LightOn(ref)
{ if(LightIsOn)  
    LightOff();
  if(ClearTimerId!=null)
    { clearTimeout(ClearTimerId);
      ClearTimerId=null;
    }
  ElementId=ElementToLight(ref);
  if(ElementId==null)
    return;
  OldColor=ElementId.style.backgroundColor;
  ElementId.style.backgroundColor="yellow";
  var _d=new Date();
  _d=_d.getTime();  
  LightStartTime=_d;
  LightIsOn=true;
}
 
function GeneralClick(event)
{ event=event || window.event;  
  var _el=event.target;
  var _tag=_el.tagName.toUpperCase();
   
   
  while(_tag!="A" && _el!=null) 
    { _el=_el.parentElement;
      if(_el!=null)
        _tag=_el.tagName.toUpperCase();
      else
        _tag="";
    }
  if(_el==null || _tag!="A")
    { if(LightIsOn)
        LightOff();
      return;
    }
  _tag=_el.getAttribute("href");
   
  if(_tag.length<2 || _tag[0]!="#")
    return;
   
  LightOn(_tag);
}
 
function GeneralMouseOver(event)
{ event=event || window.event;  
  var _tag=event.target.tagName.toUpperCase();
  if(_tag!="A")
    return;
  _tag=event.target.getAttribute("href");
   
  if(_tag.length<5 || _tag[0]!="#")
    return;
  if(_tag.substr(1,3)!="pic")  
    return;
   
  if(LitPictureId!=null)
    { LitPictureId.style.backgroundColor=LitPictureOldColor;
      LitPictureId=null;
    }
   
  _tag=_tag.substr(1);
  _tag=document.getElementById(_tag);
  if(_tag==null)
    return;
  LitPictureId=_tag;
  LitPictureOldColor=LitPictureId.style.backgroundColor;
  LitPictureId.style.backgroundColor="yellow";
}
 
function GeneralMouseOut(event)
{ event=event || window.event;  
  var _tag=event.target.tagName.toUpperCase();
  if(_tag!="A")
    return;
  _tag=event.target.getAttribute("href");
   
  if(_tag.length<5 || _tag[0]!="#")
    return;
  if(_tag.substr(1,3)!="pic")  
    return;
   
  if(LitPictureId!=null)
    { LitPictureId.style.backgroundColor=LitPictureOldColor;
      LitPictureId=null;
    }
}
 
function WindowScroll()
{ if(!LightIsOn)
    return;
   
  if(ClearTimerId!=null)  
    return;
  var _d=new Date();
  _d=_d.getTime();  
  if((_d-LightStartTime)>1000)  
    ClearTimerId=setTimeout(function(){LightOff()},5000);
}

 
function KeyDown(event)
{ event=event || window.event;  
  if(event.ctrlKey)
    { var el,_href;
      if(event.keyCode==33)  
        { el=document.getElementById("pgup") 
          if(el!=null && el.tagName.toUpperCase()=='A')
            { _href=el.getAttribute("href");
              window.location.assign(_href);
            }
        }
      else if(event.keyCode==34)  
        { el=document.getElementById("pgdn") 
          if(el!=null && el.tagName.toUpperCase()=='A')
            { _href=el.getAttribute("href");
              window.location.assign(_href);
            }
        }
    }
}
 
function DocLoad()
{
  LightIsOn=false;
  ClearTimerId=null;
  LitPictureId=null;
  LightOn(location.hash);
  window.onscroll=function() { WindowScroll(); };
  document.body.onclick=function(ev) { GeneralClick(ev); };
  document.body.onmouseover=function(ev) { GeneralMouseOver(ev); };
  document.body.onmouseout=function(ev) { GeneralMouseOut(ev); };

  document.body.onkeydown=function(ev) { KeyDown(ev); };
  
  var el,el1;
  el=document.getElementById("pgdn");
  if(el)
    { el.title=el.title+" (Ctrl+PgDn)";
      el1=document.getElementById(el.id+"_f");
      if(el1)
        el1.title=el.title;
    }
  el=document.getElementById("pgup");
  if(el)
    { el.title=el.title+" (Ctrl+PgUp)";
      el1=document.getElementById(el.id+"_f");
      if(el1)
        el1.title=el.title;
    }
}
