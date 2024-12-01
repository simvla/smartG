//-browser based graphic user inerfece app by simvla-
// v1.0 , support esp32 firmware 1.0(4 values - 3 temp(T1-text[1],T2,T3), 1 humi(H1-text[4]), 1 out map to T1(thermo function))
// use cguiapp to create symbol and text position for values, then copy the svg text to svg.txt file

var svg;

var xhr=new XMLHttpRequest();
xhr.onreadystatechange=function(){
  if(xhr.readyState!=4) return;
    if(xhr.status==200){
      svg=this.responseText; //usr
    }
  };
xhr.open("GET","svg.txt",false);
xhr.send(null);
  
//console.log(svg);

var offsetX=35;
var offsetY=55;

document.getElementById("svg").innerHTML=svg;
var text = document.getElementsByTagName('text');
console.log(text[0].textContent);
var div=document.getElementsByTagName("div");
console.log(div.length);
for(i=1;i<div.length;i++){
  let posY=parseInt(text[i-1].getAttribute("y"),10)+offsetY;
  let posX=parseInt(text[i-1].getAttribute("x"),10)+offsetX;
  //console.log(posY);
  div[i].style.position="absolute";
  div[i].style.top=posY.toString()+"px";
  div[i].style.left=posX.toString()+"px";
}

var data;
var value=[];
var time;
var local_time;

var logreq=new XMLHttpRequest();
logreq.onreadystatechange=function(){
  if(logreq.readyState!=4) return;
  if(logreq.status==200){
    data=this.responseText;
    time=new Date(); //init current time
    local_time=time.toLocaleString("en-US",{timeZone:"Europe/Budapest"});
    console.log(time);
	//console.log(data);
  }
};
logreq.open("GET","/?data",false);
logreq.send(null);

//chartjs 2.x
var log=document.getElementById("log").getContext('2d');

var chart=new Chart(log,{
  type: 'line',
  data:{
	  labels: [], //'00:00 03/04','01:00','02:00','03:00'
    datasets:[{
      label:[],
      data:[],
	    fill: false,
      backgroundColor:'white',
      borderColor:'',
      borderWidth:1
	  }]
  },
  options:{
    scales:{
      //xAxes:[{ticks:{display:false}}]
    }
  }
});

function relTimeSet(current_time,n){
	
  var hour = current_time.getHours();
  var min = current_time.getMinutes();
  var month = current_time.getMonth()+1;
  var day = current_time.getDate();
  var year = current_time.getFullYear();
  const numDays = (y, m) => new Date(y,m,0).getDate();
  //console.log(numDays(2024,11));
  console.log(hour);
  console.log(min);
  console.log(month);
  console.log(day);
  console.log(year);
  for(i=(n-1);i>0;i--){
    chart.data.labels[i-1]=hour+":"+min+" "+month+"/"+day;
    chart.update(); 
    if(hour<1) {hour=24; day-=1;}
    if(day==0) {month-=1; day=numDays(year,month);}
    //month, year?
    hour-=1;
  }
}

function formatData(sval){
  var idx1;
  var nlines=0;
  if(sval==1){
    //console.log(data.match(/\n/g).length);
	  for(i=0;i<48;i++){
      if(i==0) {
        value[i]=data.slice(0,data.indexOf(' '));
        console.log(value[i]);
        idx1=data.indexOf('\n');
      }
	    else {
        if(data.indexOf(' ',idx1+1)<idx1){nlines+=1; break;}  // prevent repeat if n rows<48
        value[i]=data.slice(idx1+1,data.indexOf(' ',idx1+1));
        console.log(value[i]);
        idx1=data.indexOf('\n',idx1+1);
        //console.log(idx1); //debug
      }
	  nlines++;
    }
	  chart.data.datasets[0].label='T1_C';
	  chart.data.datasets[0].borderColor='red';
	  chart.update();
  }
  if(sval==2){
    for(i=0;i<48;i++){
      if(i==0) {
        idx1=data.indexOf(' ');
        value[0]=data.slice(idx1+1,data.indexOf(' ',idx1+1));
        console.log(value[i]);
        idx1=data.indexOf('\n');
      }
	    else {
        if(data.indexOf(' ',idx1+1)<idx1){nlines+=1; break;}
        idx1=data.indexOf(' ',idx1);
        value[i]=data.slice(idx1+1,data.indexOf(' ',idx1+1));
        console.log(value[i]);
        idx1=data.indexOf('\n',idx1);
        
      }
	  nlines++;
    }
	  chart.data.datasets[0].label='T2_C';
	  chart.data.datasets[0].borderColor='green';
	  chart.update();
  }
  if(sval==3){
    for(i=0;i<48;i++){
      if(i==0) {
        idx1=data.indexOf(' ');
        idx1=data.indexOf(' ',idx1+1);
        value[0]=data.slice(idx1+1,data.indexOf(' ',idx1+1));
        console.log(value[i]);
        idx1=data.indexOf('\n');
      }
	    else {
        if(data.indexOf(' ',idx1+1)<idx1){nlines+=1; break;}
        idx1=data.indexOf(' ',idx1+1);
        idx1=data.indexOf(' ',idx1+1);
        value[i]=data.slice(idx1+1,data.indexOf(' ',idx1+1));
        console.log(value[i]);
        idx1=data.indexOf('\n',idx1);
        
      }
	  nlines++;
    }
	  chart.data.datasets[0].label='T3_C';
	  chart.data.datasets[0].borderColor='orange';
	  chart.update();
  }
  if(sval==4){
    for(i=0;i<48;i++){
      if(i==0) {
        idx1=data.indexOf(' ');
        idx1=data.indexOf(' ',idx1+1);
        idx1=data.indexOf(' ',idx1+1);
        value[0]=data.slice(idx1+1,data.indexOf('\n'));
        console.log(value[i]);
        idx1=data.indexOf('\n');
      }
	    else {
        if(data.indexOf(' ',idx1+1)<idx1){nlines+=1; break;}
        idx1=data.indexOf(' ',idx1+1);
        idx1=data.indexOf(' ',idx1+1);
        idx1=data.indexOf(' ',idx1+1);
        value[i]=data.slice(idx1+1,data.indexOf('\n',idx1+1));
        console.log(value[i]);
        idx1=data.indexOf('\n',idx1);
        
      }
	  nlines++
    }
	  chart.data.datasets[0].label='H1_%';
	  chart.data.datasets[0].borderColor='blue';
	  chart.update();
  }
  console.log(nlines);
  for(i=0;i<nlines;i++){
    chart.data.datasets[0].data[i]=value[i];
	  //chart.data.labels[i]=(i+1).toString();
    chart.update();
  }
  relTimeSet(time,nlines);
}

var sel=document.getElementById("sel");
sel.addEventListener('change',(event)=>{
  //console.log(sel.value);
  formatData(sel.value);
});

formatData(sel.value); //initial call


