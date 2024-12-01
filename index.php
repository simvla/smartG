<?php 
  $value="0.0";
  $html = <<<HTML
    <html>
    <head> 
      <meta name="viewport" content="width=device-width, initial-scale=1">
    </head>
    <body>
      <h2>ESP GUI Server</h2>
      <div id="svg"></div>
      <div style="">%T1%</div>
      <div style="">%T2%</div>
      <div style="">%T3%</div>
      <div style="">%H1%</div>
      <form method="GET" action="/par">
        <label>T1OFF:</label>
        <input name="toff" style="width:5%" value="{$value}"><BR>
		<label>T3OFF:</label>
		<input name="t3off" style="width:5%" value="{$value}"><BR>
        <button type="submit">SET</button>
      </form>
      <select style="margin-top:5px" id="sel">
        <option value="1">T1</option>
        <option value="2">T2</option>
        <option value="3">T3</option>
        <option value="4">H1</option>
      </select>
      <canvas id="log" width="400" height="200"></canvas>
    </body>
    <script type="text/javascript" src="Chart.min.js"></script>
    <script src="cguijs.js"></script>
    </html>
HTML;

  //echo "Hi from PHP!" 
  //$homepage=file_get_contents('http://192.168.43.138/');
  
  $base_url="http://192.168.43.138"; 
  $ctrl_url ="/par";
  $data_url ="/data";
  $PAR1="toff";
  $PAR2="t3off";
  
  function send_req($url){
    $ch = curl_init();
  
    curl_setopt($ch,CURLOPT_URL, $url);
    //curl_setopt($ch, CURLOPT_HEADER, 0);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1); 

    $result=curl_exec($ch);
    curl_close($ch);
    return $result;
  }

  if(!$_GET/*$_GET[0]==""*/ && !isset($_GET["data"]) && !isset($_GET[$PAR1])){
    //$html=send_req($base_url);
    echo $html;
    //echo $base_url;
  }

  if(isset($_GET[$PAR1])){
    $result=send_req($base_url.$ctrl_url."?".$PAR1."=".$_GET[$PAR1]."&".$PAR2."=".$_GET[$PAR2]);
    echo $result;
    //echo $html;
    //echo $base_url.$ctrl_url."?".$PAR1."=".$_GET[$PAR1]."&".$PAR2."=".$_GET[$PAR2];
    //$value=$_GET["toff"];
	//$value1=$_GET["t3off"];
    //echo $value."<BR>".$value1;
	
  }

  if(isset($_GET["data"])){
    $data=send_req($base_url.$data_url);
    echo $data;
    //echo $base_url.$data_url;
    //$myfile=fopen("data.txt","r");
    //echo fread($myfile,filesize("data.txt"));
    //fclose($myfile);
  }

  //var_dump($_GET);
  
?>