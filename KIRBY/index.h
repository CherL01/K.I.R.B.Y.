//char *ssid[] = "1306-210";
//char *pass[] = "League00..";


const char *HTML_CONTENT1 = R""""(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>K.I.R.B.Y. Controller</title>
<link rel="icon" href="data:,">
<style>
    body {
        font-family: Arial, sans-serif;
    }

    .container {
        max-width: 1000px;
        margin: 50px auto;
        text-align: center;
    }

    button {
        padding: 10px 20px;
        margin: 10px;
        font-size: 16px;
        cursor: pointer;
    }

    #incVel {
      padding: 10px;
      font-size: 16px;
      width: 100px;
      margin: 10px;
    }

    #numPlayers {
        padding: 10px;
        font-size: 16px;
        width: 100px;
        margin: 10px;
    }
</style>
</head>
<body>

<div class="container">
    <h1>K.I.R.B.Y. Controller</h1>

)"""";

const char *DEFAULT_PAGE = R""""(

    <label for="incVel">Enter Speed of Cards:</label>
    <input type="number" id="incVel" min ="1" max="10" placeholder="Speed(1~10)"><br>
    <label for="numPlayers">Enter Number of Players:</label>
    <input type="number" id="numPlayers" min="1" placeholder="Minimum 1"><br><br>

    <label for="gameType">Enter Game Playing:</label>
    <select name="gameType" id="gameType" required>
      <option value="uno">UNO<option>
      <option value="blackJack">Black Jack<option>
      <option value="bigTwo">Big 2's<option>
      <option value="poker">Poker<option>
      <option value="cheat">Cheat<option>
    </select><br>

  <button type="button" id="nextButton">Next</button><br>
  )"""";

//confirm button next to id incVel and numPlayers -> lead to page 

char *ON_PAGE = R""""(
    <h1>Game: GAME_TYPE</h1>
    <h1>Speed Setting: CARD_SPEED</h1>
    <h1># of Players: NUM_OF_PLAYERS</h1>
    <a href="/default"><button id="powerButton">EMERGENCY STOP!</button></a><br>
  )"""";

const char *START_PAGE = R""""(
  <a href="/on"><button id="powerButton">START!</button></a><br>
)"""";
    
const char *HTML_CONTENT2 = R""""(    
</div>

<script type="text/javascript">
  var powerButton = document.getElementById('powerButton');
  var cardSpeed = document.getElementById("incVel").value;
  var cardPlayer = document.getElementById("numPlayers").value;
  var gameType = document.getElementById("gameType");
  var rel_URL = '/'+String(cardSpeed)+String(cardPlayer);

  
  var submitted = document.getElementById("nextButton");

  submitted.addEventListener("click", function(event) {
    
    cardSpeed = document.getElementById("incVel").value;
    cardPlayer = document.getElementById("numPlayers").value;
    gameType = document.getElementById("gameType").value;
    
    rel_URL = '/speed'+ String(cardSpeed) + '/players' + String(cardPlayer) + '/game' + String(gameType);
    
    var abs_URL = new URL(rel_URL, window.location.href);
    window.location.replace(abs_URL);

    
});
  




</script>


</body>
</html>

)"""";

//gameType = gameType.options[gameType.selectedIndex].text;
