/*


  OK, ya ready for some fun? HTML + CSS styling + javascript all in and undebuggable environment

  one trick I've learned to how to debug HTML and CSS code.

  get all your HTML code (from html to /html) and past it into this test site
  muck with the HTML and CSS code until it's what you want
  https://www.w3schools.com/html/tryit.asp?filename=tryhtml_intro

  No clue how to debug javascrip other that write, compile, upload, refresh, guess, repeat

  I'm using class designators to set styles and id's for data updating
  for example:
  the CSS class .tabledata defines with the cell will look like
  <td><div class="tabledata" id = "switch"></div></td>

  the XML code will update the data where id = "switch"
  java script then uses getElementById
  document.getElementById("switch").innerHTML="Switch is OFF";


  .. now you can have the class define the look AND the class update the content, but you will then need
  a class for every data field that must be updated, here's what that will look like
  <td><div class="switch"></div></td>

  the XML code will update the data where class = "switch"
  java script then uses getElementsByClassName
  document.getElementsByClassName("switch")[0].style.color=text_color;


  the main general sections of a web page are the following and used here

  <html>
    <style>
    // dump CSS style stuff in here
    </style>
    <body>
      <header>
      // put header code for cute banners here
      </header>
      <main>
      // the buld of your web page contents
      </main>
      <footer>
      // put cute footer (c) 2021 xyz inc type thing
      </footer>
    </body>
    <script>
    // you java code between these tags
    </script>
  </html>


*/

// note R"KEYWORD( html page code )KEYWORD"; 
// again I hate strings, so char is it and this method let's us write naturally

const char PAGE_MAIN[] PROGMEM = R"=====(

<!DOCTYPE html>
<html lang="en" class="js-focus-visible">

<title>Web Page Update Demo</title>

  <style>
    table {
      position: relative;
      width:100%;
      border-spacing: 0px;
    }
    tr {
      border: 1px solid white;
      font-family: "Verdana", "Arial", sans-serif;
      font-size: 20px;
    }
    th {
      height: 20px;
      padding: 3px 15px;
      background-color: #343a40;
      color: #FFFFFF !important;
      }
    td {
      height: 20px;
       padding: 3px 15px;
    }
    .tabledata {
      font-size: 24px;
      position: relative;
      padding-left: 5px;
      padding-top: 5px;
      height:   25px;
      border-radius: 5px;
      color: #FFFFFF;
      line-height: 20px;
      transition: all 200ms ease-in-out;
      background-color: #00AA00;
    }
    .fanrpmslider {
      width: 30%;
      height: 55px;
      outline: none;
      height: 25px;
    }
    .bodytext {
      font-family: "Verdana", "Arial", sans-serif;
      font-size: 24px;
      text-align: left;
      font-weight: light;
      border-radius: 5px;
      display:inline;
    }
    .navbar {
      width: 100%;
      height: 50px;
      margin: 0;
      padding: 10px 0px;
      background-color: #FFF;
      color: #000000;
      border-bottom: 5px solid #293578;
    }
    .fixed-top {
      position: fixed;
      top: 0;
      right: 0;
      left: 0;
      z-index: 1030;
    }
    .navtitle {
      float: left;
      height: 50px;
      font-family: "Verdana", "Arial", sans-serif;
      font-size: 50px;
      font-weight: bold;
      line-height: 50px;
      padding-left: 20px;
    }
   .navheading {
     position: fixed;
     left: 60%;
     height: 50px;
     font-family: "Verdana", "Arial", sans-serif;
     font-size: 20px;
     font-weight: bold;
     line-height: 20px;
     padding-right: 20px;
   }
   .navdata {
      justify-content: flex-end;
      position: fixed;
      left: 70%;
      height: 50px;
      font-family: "Verdana", "Arial", sans-serif;
      font-size: 20px;
      font-weight: bold;
      line-height: 20px;
      padding-right: 20px;
   }
    .category {
      font-family: "Verdana", "Arial", sans-serif;
      font-weight: bold;
      font-size: 32px;
      line-height: 50px;
      padding: 20px 10px 0px 10px;
      color: #000000;
    }
    .heading {
      font-family: "Verdana", "Arial", sans-serif;
      font-weight: normal;
      font-size: 28px;
      text-align: left;
    }
  
    .btn {
      background-color: #444444;
      border: none;
      color: white;
      padding: 10px 20px;
      text-align: center;
      text-decoration: none;
      display: inline-block;
      font-size: 16px;
      margin: 4px 2px;
      cursor: pointer;
    }
    .foot {
      font-family: "Verdana", "Arial", sans-serif;
      font-size: 20px;
      position: relative;
      height:   30px;
      text-align: center;   
      color: #AAAAAA;
      line-height: 20px;
    }
    .container {
      max-width: 1800px;
      margin: 0 auto;
    }
    table tr:first-child th:first-child {
      border-top-left-radius: 5px;
    }
    table tr:first-child th:last-child {
      border-top-right-radius: 5px;
    }
    table tr:last-child td:first-child {
      border-bottom-left-radius: 5px;
    }
    table tr:last-child td:last-child {
      border-bottom-right-radius: 5px;
    }
    
  </style>

  <body style="background-color: #efefef" onload="process()">
  
    <header>
      <div class="navbar fixed-top">
          <div class="container">
            <div class="navtitle">Sensor Monitor</div>
            <div class="navdata" id = "date">mm/dd/yyyy</div>
            <div class="navheading">DATE</div><br>
            <div class="navdata" id = "time">00:00:00</div>
            <div class="navheading">TIME</div>
            
          </div>
      </div>
    </header>
  
    <main class="container" style="margin-top:70px">
      <div class="category">Mics Array TDOA Convolution Stream</div>
      <div style="border-radius: 10px !important;">
      <table style="width:90%">
      <colgroup>
        <col span="1" style="background-color:rgb(230,230,230); width: 20%; color:#000000 ;">
        <col span="1" style="background-color:rgb(200,200,200); width: 20%; color:#000000 ;">
        <col span="1" style="background-color:rgb(180,180,180); width: 20%; color:#000000 ;">
        <col span="1" style="background-color:rgb(180,180,180); width: 20%; color:#000000 ;">
      </colgroup>
      <col span="2"style="background-color:rgb(0,0,0); color:#FFFFFF">
      <col span="2"style="background-color:rgb(0,0,0); color:#FFFFFF">
      <col span="2"style="background-color:rgb(0,0,0); color:#FFFFFF">
      <col span="2"style="background-color:rgb(0,0,0); color:#FFFFFF">
       </tr>
        <tr>
        <td><div class="bodytext">SounDirxn,pakTym,onTime</div></td>
        <td><div class="tabledata" id = "dir"></div></td>
        <td><div class="tabledata" id = "av0"></div></td>
        <td><div class="tabledata" id = "av1"></div></td>
      </tr>
      <tr>
        <th colspan="1"><div class="heading">History</div></th>
        <th colspan="1"><div class="heading">Convolution Product</div></th>
        <th colspan="1"><div class="heading">Valid Sampls/ Total Samplz</div></th>
        <th colspan="1"><div class="heading">HFnoiz Limit/ loudness thrshold</div></th>
      </tr>
      <tr>
        <td><div class="bodytext">TDOA -29</div></td>
        <td><div class="tabledata" id = "w3"></div></td>
        <td><div class="tabledata" id = "vld"></div></td>
        <td><div class="tabledata" id = "nl"></div></td>
      </tr>
      <tr>
        <td><div class="bodytext">TDOA -27</div></td>
        <td><div class="tabledata" id = "w4"></div></td>
        <td><div class="tabledata" id = "h0"></div></td>
        <td><div class="tabledata" id = "thr"></div></td>
      </tr>
      <tr>
        <td><div class="bodytext">TDOA -25</div></td>
        <td><div class="tabledata" id = "w5"></div></td>
        <td><div class="tabledata" id = "c5"></div></td>
        <td><div class="tabledata" id = "ch5"></div></td>
      </tr>
      <tr>
        <td><div class="bodytext">TDOA -23</div></td>
        <td><div class="tabledata" id = "w6"></div></td>
        <td><div class="tabledata" id = "c6"></div></td>
        <td><div class="tabledata" id = "ch6"></div></td>
      </tr>
      <tr>
        <td><div class="bodytext">TDOA -21</div></td>
        <td><div class="tabledata" id = "w7"></div></td>
        <td><div class="tabledata" id = "c7"></div></td>
        <td><div class="tabledata" id = "ch7"></div></td>
      </tr>
      <tr>
        <td><div class="bodytext">TDOA -19</div></td>
        <td><div class="tabledata" id = "w8"></div></td>
        <td><div class="tabledata" id = "c8"></div></td>
        <td><div class="tabledata" id = "ch8"></div></td>
      </tr>
      <tr>
        <td><div class="bodytext">TDOA -17</div></td>
        <td><div class="tabledata" id = "w9"></div></td>
        <td><div class="tabledata" id = "c9"></div></td>
        <td><div class="tabledata" id = "ch9"></div></td>
      </tr>
      <tr>
      <td><div class="bodytext">TDOA -15</div></td>
        <td><div class="tabledata" id = "w10"></div></td>
        <td><div class="tabledata" id = "c10"></div></td>
        <td><div class="tabledata" id = "ch10"></div></td>
      </tr>
      <tr>
        <td><div class="bodytext">TDOA -13</div></td>
        <td><div class="tabledata" id = "w11"></div></td>
        <td><div class="tabledata" id = "c11"></div></td>
        <td><div class="tabledata" id = "ch11"></div></td>
      </tr>
      <tr>
        <td><div class="bodytext">TDOA -11</div></td>
        <td><div class="tabledata" id = "w12"></div></td>
        <td><div class="tabledata" id = "c12"></div></td>
        <td><div class="tabledata" id = "ch12"></div></td>
      </tr>
      <tr>
        <td><div class="bodytext">TDOA -9</div></td>
        <td><div class="tabledata" id = "w13"></div></td>
        <td><div class="tabledata" id = "c13"></div></td>
        <td><div class="tabledata" id = "ch13"></div></td>
      </tr>
      <tr>
        <td><div class="bodytext">TDOA -7</div></td>
        <td><div class="tabledata" id = "w14"></div></td>
        <td><div class="tabledata" id = "c14"></div></td>
        <td><div class="tabledata" id = "ch14"></div></td>
      </tr>
      <tr>
        <td><div class="bodytext">TDOA -5</div></td>
        <td><div class="tabledata" id = "w15"></div></td>
        <td><div class="tabledata" id = "c15"></div></td>
        <td><div class="tabledata" id = "ch15"></div></td>
      </tr>
      <tr>
        <td><div class="bodytext">TDOA -3</div></td>
        <td><div class="tabledata" id = "w16"></div></td>
        <td><div class="tabledata" id = "c16"></div></td>
        <td><div class="tabledata" id = "ch16"></div></td>
      </tr>
      <tr>
        <td><div class="bodytext">TDOA -1</div></td>
        <td><div class="tabledata" id = "w17"></div></td>
        <td><div class="tabledata" id = "c17"></div></td>
        <td><div class="tabledata" id = "ch17"></div></td>
      </tr>
      <tr>
        <td><div class="bodytext">TDOA +1</div></td>
        <td><div class="tabledata" id = "w18"></div></td>
        <td><div class="tabledata" id = "c18"></div></td>
        <td><div class="tabledata" id = "ch18"></div></td>
      </tr>
      <tr>
        <td><div class="bodytext">TDOA +3</div></td>
        <td><div class="tabledata" id = "w19"></div></td>
        <td><div class="tabledata" id = "c19"></div></td>
        <td><div class="tabledata" id = "ch19"></div></td>
      </tr>
      <tr>
      <td><div class="bodytext">TDOA +5</div></td>
        <td><div class="tabledata" id = "w20"></div></td>
        <td><div class="tabledata" id = "c20"></div></td>
        <td><div class="tabledata" id = "ch20"></div></td>
      </tr>
      <tr>
        <td><div class="bodytext">TDOA +7</div></td>
        <td><div class="tabledata" id = "w21"></div></td>
        <td><div class="tabledata" id = "c21"></div></td>
        <td><div class="tabledata" id = "ch21"></div></td>
      </tr>
      <tr>
      <td><div class="bodytext">TDOA +9</div></td>
        <td><div class="tabledata" id = "w22"></div></td>
        <td><div class="tabledata" id = "c22"></div></td>
        <td><div class="tabledata" id = "ch22"></div></td>
      </tr>
      <tr>
        <td><div class="bodytext">TDOA +11</div></td>
        <td><div class="tabledata" id = "w23"></div></td>
        <td><div class="tabledata" id = "c23"></div></td>
        <td><div class="tabledata" id = "ch23"></div></td>
      </tr>
       <tr>
        <td><div class="bodytext">TDOA +13</div></td>
        <td><div class="tabledata" id = "w24"></div></td>
        <td><div class="tabledata" id = "c24"></div></td>
        <td><div class="tabledata" id = "ch24"></div></td>
      </tr>
      <tr>
        <td><div class="bodytext">TDOA +15</div></td>
        <td><div class="tabledata" id = "w25"></div></td>
        <td><div class="tabledata" id = "c25"></div></td>
        <td><div class="tabledata" id = "ch25"></div></td>
      </tr>
      <tr>
        <td><div class="bodytext">TDOA +17</div></td>
        <td><div class="tabledata" id = "w26"></div></td>
        <td><div class="tabledata" id = "c26"></div></td>
        <td><div class="tabledata" id = "ch26"></div></td>
      </tr>
      <tr>
        <td><div class="bodytext">TDOA +19</div></td>
        <td><div class="tabledata" id = "w27"></div></td>
        <td><div class="tabledata" id = "c27"></div></td>
        <td><div class="tabledata" id = "ch27"></div></td>
      </tr>
      <tr>
        <td><div class="bodytext">TDOA +21</div></td>
        <td><div class="tabledata" id = "w28"></div></td>
        <td><div class="tabledata" id = "c28"></div></td>
        <td><div class="tabledata" id = "ch28"></div></td>
      </tr>
      <tr>
        <td><div class="bodytext">TDOA +23</div></td>
        <td><div class="tabledata" id = "w29"></div></td>
        <td><div class="tabledata" id = "c29"></div></td>
        <td><div class="tabledata" id = "ch29"></div></td>
      </tr>
      <tr>
      <td><div class="bodytext">TDOA +25</div></td>
        <td><div class="tabledata" id = "w30"></div></td>
        <td><div class="tabledata" id = "c30"></div></td>
        <td><div class="tabledata" id = "ch30"></div></td>
      </tr>
      <tr>
        <td><div class="bodytext">TDOA +27</div></td>
        <td><div class="tabledata" id = "w31"></div></td>
        <td><div class="tabledata" id = "c31"></div></td>
        <td><div class="tabledata" id = "ch31"></div></td>
      </tr>
      <tr>
      <td><div class="bodytext">TDOA +29</div></td>
        <td><div class="tabledata" id = "w32"></div></td>
        <td><div class="tabledata" id = "c32"></div></td>
        <td><div class="tabledata" id = "ch32"></div></td>
      </tr>
      </table>
    </div>
    <br>
    <br>
  </main>

  <footer div class="foot" id = "temp" >ESP32 Web Page Creation and Data Update Demo</div></footer>
  
  </body>

  <script type = "text/javascript">
  
    // global variable visible to all java functions
    var xmlHttp=createXmlHttpObject();


    // function to create XML object
    function createXmlHttpObject(){
      if(window.XMLHttpRequest){
        xmlHttp=new XMLHttpRequest();
      }
      else{
        xmlHttp=new ActiveXObject("Microsoft.XMLHTTP");
      }
      return xmlHttp;
    }

    // function to handle button press from HTML code above
    // and send a processing string back to server
    // this processing string is use in the .on method
    function ButtonPress0() {
      var xhttp = new XMLHttpRequest(); 
      var message;
      // if you want to handle an immediate reply (like status from the ESP
      // handling of the button press use this code
      // since this button status from the ESP is in the main XML code
      // we don't need this
      // remember that if you want immediate processing feedbac you must send it
      // in the ESP handling function and here
      /*
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          message = this.responseText;
          // update some HTML data
        }
      }
      */
       
      xhttp.open("PUT", "BUTTON_0", false);
      xhttp.send();
    }


    // function to handle button press from HTML code above
    // and send a processing string back to server
    // this processing string is use in the .on method
    function ButtonPress1() {
      var xhttp = new XMLHttpRequest(); 
      /*
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("button1").innerHTML = this.responseText;
        }
      }
      */
      xhttp.open("PUT", "BUTTON_1", false);
      xhttp.send(); 
    }
    
    function UpdateSlider(value) {
      var xhttp = new XMLHttpRequest();
      // this time i want immediate feedback to the fan speed
      // yea yea yea i realize i'm computing fan speed but the point
      // is how to give immediate feedback
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          // update the web based on reply from  ESP
          document.getElementById("fanrpm").innerHTML=this.responseText;
        }
      }
      // this syntax is really weird the ? is a delimiter
      // first arg UPDATE_SLIDER is use in the .on method
      // server.on("/UPDATE_SLIDER", UpdateSlider);
      // then the second arg VALUE is use in the processing function
      // String t_state = server.arg("VALUE");
      // then + the controls value property
      xhttp.open("PUT", "UPDATE_SLIDER?VALUE="+value, true);
      xhttp.send();
    }

    // function to handle the response from the ESP
    function response(){
      var message;
      var wHi;
      var barwidth;
      var currentsensor;
      var xmlResponse;
      var xmldoc;
      var dt = new Date();
      var color = "#e8e8e8";
      var barwidth = 9.9;
      var gutter = 40;
      var prev;
      var mrk;


      // get the xml stream
      xmlResponse=xmlHttp.responseXML;
  
      // get host date and time
      document.getElementById("time").innerHTML = dt.toLocaleTimeString();
      document.getElementById("date").innerHTML = dt.toLocaleDateString();
  
      barwidth = 20;

      //xmldoc = xmlResponse.getElementsByTagName("B0"); //volts for A0
      //wHi = xmldoc[0].firstChild.nodeValue;
      //message = xmldoc[0].firstChild.nodeValue;

      //if(message > 0){
       // barwidth = message/gutter;
       // if(barwidth > 100){
       //   gutter*=(barwidth/100);
       //   barwidth = 100;
      //  }
     // }
     // document.getElementById("dir").style.width=(barwidth+"%");
      //document.getElementById("dir").innerHTML = message;

      xmldoc = xmlResponse.getElementsByTagName("WH"); //volts for A0
      wHi = xmldoc[0].firstChild.nodeValue;
      //wHi = wHi + 36; //doesn't work for i dunno why   
        document.getElementById("dir").style.width=(barwidth+"%");
      document.getElementById("dir").innerHTML = wHi;

 xmldoc = xmlResponse.getElementsByTagName("VLD"); //volts for A0
      message = xmldoc[0].firstChild.nodeValue;
      if(message > 0){
        barwidth = message/gutter;
        if(barwidth > 100){
          gutter*=(barwidth/100);
          barwidth = 100;
        }
      }
      document.getElementById("vld").style.width=(barwidth+"%");
      document.getElementById("vld").innerHTML = message;

 xmldoc = xmlResponse.getElementsByTagName("H0"); //volts for A0
      message = xmldoc[0].firstChild.nodeValue;
      if(message > 0){
        barwidth = message/gutter;
        if(barwidth > 100){
          gutter*=(barwidth/100);
          barwidth = 100;
        }
      }
      document.getElementById("h0").style.width=(barwidth+"%");
      document.getElementById("h0").innerHTML = message;

      xmldoc = xmlResponse.getElementsByTagName("AVJ0"); //volts for A0
      message = xmldoc[0].firstChild.nodeValue;
      if(message > 0){
        barwidth = message/gutter;
        if(barwidth > 100){
          gutter*=(barwidth/100);
          barwidth = 100;
        }
      }
      document.getElementById("av0").style.width=(barwidth+"%");
      document.getElementById("av0").innerHTML = message;

      xmldoc = xmlResponse.getElementsByTagName("AVJ1"); //volts for A0
      message = xmldoc[0].firstChild.nodeValue;
      barwidth = message/50;
      if(message > 0){
        barwidth = message/gutter;
        if(barwidth > 100){
          gutter*=(barwidth/100);
          barwidth = 100;
        }
      }
      document.getElementById("av1").style.width=(barwidth+"%");
      document.getElementById("av1").innerHTML = message;

      xmldoc = xmlResponse.getElementsByTagName("NL"); //volts for A0
      message = xmldoc[0].firstChild.nodeValue;
      barwidth = message/50;
      if(message > 0){
        barwidth = message/gutter;
        if(barwidth > 100){
          gutter*=(barwidth/100);
          barwidth = 100;
        }
      }
      document.getElementById("nl").style.width=(barwidth+"%");
      document.getElementById("nl").innerHTML = message;

      xmldoc = xmlResponse.getElementsByTagName("THR"); //volts for A0
      message = xmldoc[0].firstChild.nodeValue;
      barwidth = message/50;
      if(message > 0){
        barwidth = message/gutter;
        if(barwidth > 100){
          gutter*=(barwidth/100);
          barwidth = 100;
        }
      }
      document.getElementById("thr").style.width=(barwidth+"%");
      document.getElementById("thr").innerHTML = message;

      xmldoc = xmlResponse.getElementsByTagName("W3"); //volts for A0
      message = xmldoc[0].firstChild.nodeValue;
      barwidth = 0;
      if(message > 0){
        barwidth = message/gutter;
        if(barwidth > 100){
          gutter*=(barwidth/100);
          barwidth = 100;
        }
      }
      document.getElementById("w3").style.width=(barwidth+"%");
      document.getElementById("w3").innerHTML = message;
      //if(wHi >= 6 && wHi < 8){
      if(wHi >= -30 && wHi < -28){
        color = "#FF0800";
      }
      else{
        color = "#00FF00";
      }
      document.getElementById("w3").style.backgroundColor=color;

      xmldoc = xmlResponse.getElementsByTagName("W4"); //volts for A0
      message = xmldoc[0].firstChild.nodeValue;
      barwidth = 0;
      if(message > 0){
        barwidth = message/gutter;
        if(barwidth > 100){
          gutter*=(barwidth/100);
          barwidth = 100;
        }
      }
      document.getElementById("w4").style.width=(barwidth+"%");
      document.getElementById("w4").innerHTML = message;
      //if(wHi >= 8 && wHi < 10){
      if(wHi >= -28 && wHi < -26){
        color = "#FF0800";
      }
      else{
        color = "#00FF00";
      }
      document.getElementById("w4").style.backgroundColor=color;

      xmldoc = xmlResponse.getElementsByTagName("W5"); //volts for A0
      message = xmldoc[0].firstChild.nodeValue;
      barwidth = 0;
      if(message > 0){
        barwidth = message/gutter;
        if(barwidth > 100){
          gutter*=(barwidth/100);
          barwidth = 100;
        }
      }
      document.getElementById("w5").style.width=(barwidth+"%");
      document.getElementById("w5").innerHTML = message;
      //if(wHi >= 10 && wHi < 12){
      if(wHi >= -26 && wHi < -24){
        color = "#FF0800";
      }
      else{
        color = "#00FF00";
      }
      document.getElementById("w5").style.backgroundColor=color;
      
      xmldoc = xmlResponse.getElementsByTagName("W6"); //volts for A0
      message = xmldoc[0].firstChild.nodeValue;
      barwidth = 0;
      if(message > 0){
        barwidth = message/gutter;
        if(barwidth > 100){
          gutter*=(barwidth/100);
          barwidth = 100;
        }
      }
      document.getElementById("w6").style.width=(barwidth+"%");
      document.getElementById("w6").innerHTML = message;
      //if(wHi >= 12 && wHi < 14){
      if(wHi >= -24 && wHi < -22){
        color = "#FF0800";
      }
      else{
        color = "#00FF00";
      }
      document.getElementById("w6").style.backgroundColor=color;

      xmldoc = xmlResponse.getElementsByTagName("W7"); //volts for A0
      message = xmldoc[0].firstChild.nodeValue;
      barwidth = 0;
      if(message > 0){
        barwidth = message/gutter;
        if(barwidth > 100){
          gutter*=(barwidth/100);
          barwidth = 100;
        }
      }
      document.getElementById("w7").style.width=(barwidth+"%");
      document.getElementById("w7").innerHTML = message;
      //if(wHi >= 14 && wHi < 16){
      if(wHi >= -22 && wHi < -20){
        color = "#FF0800";
      }
      else{
        color = "#00FF00";
      }
      document.getElementById("w7").style.backgroundColor=color;

      xmldoc = xmlResponse.getElementsByTagName("W8"); //volts for A0
      message = xmldoc[0].firstChild.nodeValue;
      barwidth = 0;
      if(message > 0){
        barwidth = message/gutter;
        if(barwidth > 100){
          gutter*=(barwidth/100);
          barwidth = 100;
        }
      }
      document.getElementById("w8").style.width=(barwidth+"%");
      document.getElementById("w8").innerHTML = message;
      //if(wHi >= 16 && wHi < 18){
      if(wHi >= -20 && wHi < -18){
        color = "#FF0800";
      }
      else{
        color = "#00FF00";
      }
      document.getElementById("w8").style.backgroundColor=color;

      xmldoc = xmlResponse.getElementsByTagName("W9"); //volts for A0
      message = xmldoc[0].firstChild.nodeValue;
      barwidth = 0;
      if(message > 0){
        barwidth = message/gutter;
        if(barwidth > 100){
          gutter*=(barwidth/100);
          barwidth = 100;
        }
      }
      document.getElementById("w9").style.width=(barwidth+"%");
      document.getElementById("w9").innerHTML = message;
      //if(wHi >= 18 && wHi < 20){
      if(wHi >= -18 && wHi < -16){
        color = "#FF0800";
      }
      else{
        color = "#00FF00";
      }
      document.getElementById("w9").style.backgroundColor=color;
      
      xmldoc = xmlResponse.getElementsByTagName("W10"); //volts for A0
      message = xmldoc[0].firstChild.nodeValue;
      barwidth = 0;
      if(message > 0){
        barwidth = message/gutter;
        if(barwidth > 100){
          gutter*=(barwidth/100);
          barwidth = 100;
        }
      }
      document.getElementById("w10").style.width=(barwidth+"%");
      document.getElementById("w10").innerHTML = message;
      //if(wHi >= 20 && wHi < 22){
      if(wHi >= -16 && wHi < -14){
        color = "#FF0800";
      }
      else{
        color = "#00FF00";
      }
      document.getElementById("w10").style.backgroundColor=color;

      xmldoc = xmlResponse.getElementsByTagName("W11"); //volts for A0
      message = xmldoc[0].firstChild.nodeValue;
      barwidth = 0;
      if(message > 0){
        barwidth = message/gutter;
        if(barwidth > 100){
          gutter*=(barwidth/100);
          barwidth = 100;
        }
      }
      document.getElementById("w11").style.width=(barwidth+"%");
      document.getElementById("w11").innerHTML = message;
      //if(wHi >= 22 && wHi < 24){
      if(wHi >= -14 && wHi < -12){
        color = "#FF0800";
      }
      else{
        color = "#00FF00";
      }
      document.getElementById("w11").style.backgroundColor=color;

      xmldoc = xmlResponse.getElementsByTagName("W12"); //volts for A0
      message = xmldoc[0].firstChild.nodeValue;
      barwidth = 0;
      if(message > 0){
        barwidth = message/gutter;
        if(barwidth > 100){
          gutter*=(barwidth/100);
          barwidth = 100;
        }
      }
      document.getElementById("w12").style.width=(barwidth+"%");
      document.getElementById("w12").innerHTML = message;
      //if(wHi >= 24 && wHi < 26){
      if(wHi >= -12 && wHi < -10){
        color = "#FF0800";
      }
      else{
        color = "#00FF00";
      }
      document.getElementById("w12").style.backgroundColor=color;

      xmldoc = xmlResponse.getElementsByTagName("W13"); //volts for A0
      message = xmldoc[0].firstChild.nodeValue;
      barwidth = 0;
      if(message > 0){
        barwidth = message/gutter;
        if(barwidth > 100){
          gutter*=(barwidth/100);
          barwidth = 100;
        }
      }
      document.getElementById("w13").style.width=(barwidth+"%");
      document.getElementById("w13").innerHTML = message;
      //if(wHi >= 26 && wHi < 28){
      if(wHi >= -10 && wHi < -8){
        color = "#FF0800";
      }
      else{
        color = "#00FF00";
      }
      document.getElementById("w13").style.backgroundColor=color;
      
      xmldoc = xmlResponse.getElementsByTagName("W14"); //volts for A0
      message = xmldoc[0].firstChild.nodeValue;
      barwidth = 0;
      if(message > 0){
        barwidth = message/gutter;
        if(barwidth > 100){
          gutter*=(barwidth/100);
          barwidth = 100;
        }
      }
      document.getElementById("w14").style.width=(barwidth+"%");
      document.getElementById("w14").innerHTML = message;
      //if(wHi >= 28 && wHi < 30){
      if(wHi >= -8 && wHi < -6){
        color = "#FF0800";
      }
      else{
        color = "#00FF00";
      }
      document.getElementById("w14").style.backgroundColor=color;

      xmldoc = xmlResponse.getElementsByTagName("W15"); //volts for A0
      message = xmldoc[0].firstChild.nodeValue;
      barwidth = 0;
      if(message > 0){
        barwidth = message/gutter;
        if(barwidth > 100){
          gutter*=(barwidth/100);
          barwidth = 100;
        }
      }
      document.getElementById("w15").style.width=(barwidth+"%");
      document.getElementById("w15").innerHTML = message;
      //if(wHi >=30 && wHi < 32){
      if(wHi >= -6 && wHi < -4){
        color = "#FF0800";
      }
      else{
        color = "#00FF00";
      }
      document.getElementById("w15").style.backgroundColor=color;

      xmldoc = xmlResponse.getElementsByTagName("W16"); //volts for A0
      message = xmldoc[0].firstChild.nodeValue;
      barwidth = 0;
      if(message > 0){
        barwidth = message/gutter;
        if(barwidth > 100){
          gutter*=(barwidth/100);
          barwidth = 100;
        }
      }
      document.getElementById("w16").style.width=(barwidth+"%");
      document.getElementById("w16").innerHTML = message;
      //if(wHi >= 32 && wHi < 34){
      if(wHi >= -4 && wHi < -2){
        color = "#FF0800";
      }
      else{
        color = "#00FF00";
      }
      document.getElementById("w16").style.backgroundColor=color;

      xmldoc = xmlResponse.getElementsByTagName("W17"); //volts for A0
      message = xmldoc[0].firstChild.nodeValue;
      barwidth = 0;
      if(message > 0){
        barwidth = message/gutter;
        if(barwidth > 100){
          gutter*=(barwidth/100);
          barwidth = 100;
        }
      }
      document.getElementById("w17").style.width=(barwidth+"%");
      document.getElementById("w17").innerHTML = message;
      //if(wHi >= 34 && wHi < 36){
      if(wHi >= -2 && wHi < 0){
        color = "#FF0800";
      }
      else{
        color = "#00FF00";
      }
      document.getElementById("w17").style.backgroundColor=color;
      
      xmldoc = xmlResponse.getElementsByTagName("W18"); //volts for A0
      message = xmldoc[0].firstChild.nodeValue;
      barwidth = 0;
      if(message > 0){
        barwidth = message/gutter;
        if(barwidth > 100){
          gutter*=(barwidth/100);
          barwidth = 100;
        }
      }
      document.getElementById("w18").style.width=(barwidth+"%");
      document.getElementById("w18").innerHTML = message;
       if(wHi >= 0 && wHi < 2){
        color = "#FF0800";
      }
      else{
        color = "#00FF00";
      }
      document.getElementById("w18").style.backgroundColor=color;

      xmldoc = xmlResponse.getElementsByTagName("W19"); //volts for A0
      message = xmldoc[0].firstChild.nodeValue;
      barwidth = 0;
      if(message > 0){
        barwidth = message/gutter;
        if(barwidth > 100){
          gutter*=(barwidth/100);
          barwidth = 100;
        }
      }
      document.getElementById("w19").style.width=(barwidth+"%");
      document.getElementById("w19").innerHTML = message;
       if(wHi >= 2 && wHi < 4){
        color = "#FF0800";
      }
      else{
        color = "#00FF00";
      }
      document.getElementById("w19").style.backgroundColor=color;

      xmldoc = xmlResponse.getElementsByTagName("W20"); //volts for A0
      message = xmldoc[0].firstChild.nodeValue;
      barwidth = 0;
      if(message > 0){
        barwidth = message/gutter;
        if(barwidth > 100){
          gutter*=(barwidth/100);
          barwidth = 100;
        }
      }
      document.getElementById("w20").style.width=(barwidth+"%");
      document.getElementById("w20").innerHTML = message;
       if(wHi >= 4 && wHi < 6){
        color = "#FF0800";
      }
      else{
        color = "#00FF00";
      }
      document.getElementById("w20").style.backgroundColor=color;

      xmldoc = xmlResponse.getElementsByTagName("W21"); //volts for A0
      message = xmldoc[0].firstChild.nodeValue;
      barwidth = 0;
      if(message > 0){
        barwidth = message/gutter;
        if(barwidth > 100){
          gutter*=(barwidth/100);
          barwidth = 100;
        }
      }
      document.getElementById("w21").style.width=(barwidth+"%");
      document.getElementById("w21").innerHTML = message;
       if(wHi >= 6 && wHi < 8){
        color = "#FF0800";
      }
      else{
        color = "#00FF00";
      }
      document.getElementById("w21").style.backgroundColor=color;
      
      xmldoc = xmlResponse.getElementsByTagName("W22"); //volts for A0
      message = xmldoc[0].firstChild.nodeValue;
      barwidth = 0;
      if(message > 0){
        barwidth = message/gutter;
        if(barwidth > 100){
          gutter*=(barwidth/100);
          barwidth = 100;
        }
      }
      document.getElementById("w22").style.width=(barwidth+"%");
      document.getElementById("w22").innerHTML = message;
       if(wHi >= 8 && wHi < 10){
        color = "#FF0800";
      }
      else{
        color = "#00FF00";
      }
      document.getElementById("w22").style.backgroundColor=color;

      xmldoc = xmlResponse.getElementsByTagName("W23"); //volts for A0
      message = xmldoc[0].firstChild.nodeValue;
      barwidth = 0;
      if(message > 0){
        barwidth = message/gutter;
        if(barwidth > 100){
          gutter*=(barwidth/100);
          barwidth = 100;
        }
      }
      document.getElementById("w23").style.width=(barwidth+"%");
      document.getElementById("w23").innerHTML = message;
       if(wHi >= 10 && wHi < 12){
        color = "#FF0800";
      }
      else{
        color = "#00FF00";
      }
      document.getElementById("w23").style.backgroundColor=color;

      xmldoc = xmlResponse.getElementsByTagName("W24"); //volts for A0
      message = xmldoc[0].firstChild.nodeValue;
      barwidth = 0;
      if(message > 0){
        barwidth = message/gutter;
        if(barwidth > 100){
          gutter*=(barwidth/100);
          barwidth = 100;
        }
      }
      document.getElementById("w24").style.width=(barwidth+"%");
      document.getElementById("w24").innerHTML = message;
      if(wHi >= 12 && wHi < 14){
        color = "#FF0800";
      }
      else{
        color = "#00FF00";
      }
      document.getElementById("w24").style.backgroundColor=color;

      xmldoc = xmlResponse.getElementsByTagName("W25"); //volts for A0
      message = xmldoc[0].firstChild.nodeValue;
      barwidth = 0;
      if(message > 0){
        barwidth = message/gutter;
        if(barwidth > 100){
          gutter*=(barwidth/100);
          barwidth = 100;
        }
      }
      document.getElementById("w25").style.width=(barwidth+"%");
      document.getElementById("w25").innerHTML = message;
      if(wHi >= 14 && wHi < 16){
        color = "#FF0800";
      }
      else{
        color = "#00FF00";
      }
      document.getElementById("w25").style.backgroundColor=color;
      
      xmldoc = xmlResponse.getElementsByTagName("W26"); //volts for A0
      message = xmldoc[0].firstChild.nodeValue;
      barwidth = 0;
      if(message > 0){
        barwidth = message/gutter;
        if(barwidth > 100){
          gutter*=(barwidth/100);
          barwidth = 100;
        }
      }
      document.getElementById("w26").style.width=(barwidth+"%");
      document.getElementById("w26").innerHTML = message;
      if(wHi >= 16 && wHi < 18){
        color = "#FF0800";
      }
      else{
        color = "#00FF00";
      }
      document.getElementById("w26").style.backgroundColor=color;

      xmldoc = xmlResponse.getElementsByTagName("W27"); //volts for A0
      message = xmldoc[0].firstChild.nodeValue;
      barwidth = 0;
      if(message > 0){
        barwidth = message/gutter;
        if(barwidth > 100){
          gutter*=(barwidth/100);
          barwidth = 100;
        }
      }
      document.getElementById("w27").style.width=(barwidth+"%");
      document.getElementById("w27").innerHTML = message;
      if(wHi >= 18 && wHi < 20){
        color = "#FF0800";
      }
      else{
        color = "#00FF00";
      }
      document.getElementById("w27").style.backgroundColor=color;

      xmldoc = xmlResponse.getElementsByTagName("W28"); //volts for A0
      message = xmldoc[0].firstChild.nodeValue;
      barwidth = 0;
      if(message > 0){
        barwidth = message/gutter;
        if(barwidth > 100){
          gutter*=(barwidth/100);
          barwidth = 100;
        }
      }
      document.getElementById("w28").style.width=(barwidth+"%");
      document.getElementById("w28").innerHTML = message;
      if(wHi >= 20 && wHi < 22){
        color = "#FF0800";
      }
      else{
        color = "#00FF00";
      }
      document.getElementById("w28").style.backgroundColor=color;

      xmldoc = xmlResponse.getElementsByTagName("W29"); //volts for A0
      message = xmldoc[0].firstChild.nodeValue;
      barwidth = 0;
      if(message > 0){
        barwidth = message/gutter;
        if(barwidth > 100){
          gutter*=(barwidth/100);
          barwidth = 100;
        }
      }
      document.getElementById("w29").style.width=(barwidth+"%");
      document.getElementById("w29").innerHTML = message;
      if(wHi >= 22 && wHi < 24){
        color = "#FF0800";
      }
      else{
        color = "#00FF00";
      }
      document.getElementById("w29").style.backgroundColor=color;
      
      xmldoc = xmlResponse.getElementsByTagName("W30"); //volts for A0
      message = xmldoc[0].firstChild.nodeValue;
      barwidth = 0;
      if(message > 0){
        barwidth = message/gutter;
        if(barwidth > 100){
          gutter*=(barwidth/100);
          barwidth = 100;
        }
      }
      document.getElementById("w30").style.width=(barwidth+"%");
      document.getElementById("w30").innerHTML = message;
      if(wHi >= 24 && wHi < 26){
        color = "#FF0800";
      }
      else{
        color = "#00FF00";
      }
      document.getElementById("w30").style.backgroundColor=color;

      xmldoc = xmlResponse.getElementsByTagName("W31"); //volts for A0
      message = xmldoc[0].firstChild.nodeValue;
      barwidth = 0;
      if(message > 0){
        barwidth = message/gutter;
        if(barwidth > 100){
          gutter*=(barwidth/100);
          barwidth = 100;
        }
      }
      document.getElementById("w31").style.width=(barwidth+"%");
      document.getElementById("w31").innerHTML = message;
      if(wHi >= 26 && wHi < 28){
        color = "#FF0800";
      }
      else{
        color = "#00FF00";
      }
      document.getElementById("w31").style.backgroundColor=color;

      xmldoc = xmlResponse.getElementsByTagName("W32"); //volts for A0
      message = xmldoc[0].firstChild.nodeValue;
      barwidth = 0;
      if(message > 0){
        barwidth = message/gutter;
        if(barwidth > 100){
          gutter*=(barwidth/100);
          barwidth = 100;
        }
      }
      document.getElementById("w32").style.width=(barwidth+"%");
      document.getElementById("w32").innerHTML = message;
      if(wHi >= 28 && wHi < 30){
        color = "#FF0800";
      }
      else{
        color = "#00FF00";
      }
      document.getElementById("w32").style.backgroundColor=color;   

     // xmldoc = xmlResponse.getElementsByTagName("WH36"); //volts for A0
     // message = xmldoc[0].firstChild.nodeValue;
     // barwidth = 0;
     // if(message > 0){
     //   barwidth = message/gutter;
      //}
     // document.getElementById("c36").style.width=(barwidth+"%");
     // document.getElementById("c36").innerHTML = message;

    // xmldoc = xmlResponse.getElementsByTagName("WH37"); //volts for A0
     // message = xmldoc[0].firstChild.nodeValue;
     // barwidth = 0;

     // if(message > 0){
     //   barwidth = message/gutter;
     // }
      //document.getElementById("c37").style.width=(barwidth+"%");
     // document.getElementById("c37").innerHTML = message;
      
     // xmldoc = xmlResponse.getElementsByTagName("WH38"); //volts for A0
     // message = xmldoc[0].firstChild.nodeValue;
     // barwidth = 0;
     // if(message > 0){
     //   barwidth = message/gutter;
     // }
     // document.getElementById("c38").style.width=(barwidth+"%");
     // document.getElementById("c38").innerHTML = message;

     // xmldoc = xmlResponse.getElementsByTagName("WH39"); //volts for A0
     // message = xmldoc[0].firstChild.nodeValue;
     // barwidth = 0;
     // if(message > 0){
     //   barwidth = message/gutter;
     // }
      //document.getElementById("c39").style.width=(barwidth+"%");
     // document.getElementById("c39").innerHTML = message;
    
    }
  
    // general processing code for the web page to ask for an XML steam
    // this is actually why you need to keep sending data to the page to 
    // force this call with stuff like this
    // server.on("/xml", SendXML);
    // otherwise the page will not request XML from the ESP, and updates will not happen
    function process(){
     
     if(xmlHttp.readyState==0 || xmlHttp.readyState==4) {
        xmlHttp.open("PUT","xml",true);
        xmlHttp.onreadystatechange=response;
        xmlHttp.send(null);
      }       
        // you may have to play with this value, big pages need more porcessing time, and hence
        // a longer timeout
        setTimeout("process()", 250);
    }
  
  
  </script>

</html>



)=====";