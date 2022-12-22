


#include <WiFi.h>
#include <WiFiClient.h>
#include <UniversalTelegramBot.h>   // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <ArduinoJson.h>
#include <HTTPClient.h>



// Replace with your network credentials
const char* ssid = "motorola";
const char* password = "motorola";
String GOOGLE_SCRIPT_ID="AKfycbxqKOLF_5zlsbcKoZbLqNx7t0rfenNbAf7H7LUR3S7e6zSt9u9LTlU2knQLYrTCoZQ";

// Initialize Telegram BOT
#define BOTtoken "5647527330:AAHvsIGSxLxNx8WHACr7Ms35j4UYnNLMzGs"  // your Bot Token (Get from Botfather)

// Use @myidbot to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you
#define CHAT_ID "687022702"


WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

const int ledPin = 2;
bool ledState = LOW;

String write_into_Spreadsheet(String user_name, String password  )
{
  
  if(WiFi.status()==WL_CONNECTED)
  {
    
        String urlFinal = "https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?user_name="+String(user_name)+"&password="+String(password)+"&command="+String("write_user");
        HTTPClient http;
        Serial.println("Making a request");
   
    http.begin(urlFinal.c_str()); //Specify the URL and certificate
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpCode = http.GET();
        
    String payload;
    if (httpCode ==200) { //Check for the returning code
      payload = http.getString();
      Serial.println(httpCode);
      Serial.println(payload);
    }
    else {
      Serial.println("Error on HTTP request");
    }
    http.end();

    return payload;
    
  }

  
}


String read_data(String unique_id, String parameter)
{
    if(parameter=="unique_id")
    {
      parameter="A";
    }

    else if(parameter=="user_name")
    {
        parameter="B";
    }
    else if(parameter=="password")
    {
        parameter="C";
    }
    else if(parameter=="available_balance")
    {
        parameter="D";
    }
    else if(parameter=="transaction_history")
    {
      parameter="E";
    }
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        
        String loc=parameter+unique_id;
        String url = "https://script.google.com/macros/s/"+ GOOGLE_SCRIPT_ID + "/exec?cell_location="+loc+"&command=read";
        Serial.println("Making a request");
        http.begin(url.c_str()); //Specify the URL and certificate
        http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
        int httpCode = http.GET();
        
        String payload;
    if (httpCode ==200) { //Check for the returning code
      payload = http.getString();
      Serial.println(httpCode);
      Serial.println(payload);
    }
    else {
      Serial.println("Error on HTTP request");
    }
    http.end();

    return payload;
}
}
void update(String unique_id, String parameter)
{
  if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        String url = "https://script.google.com/macros/s/"+ GOOGLE_SCRIPT_ID + "/exec?cell_location=D"+String(unique_id)+"&update_balance="+String(parameter)+"&command=update_balance";
        Serial.println("Making a request");
        http.begin(url.c_str()); //Specify the URL and certificate
        http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
        int httpCode = http.GET();
        Serial.print("HTTP status code");
        Serial.println(httpCode);
        http.end();
        
  }


}
void get_update_Summary(String unique_id, String parameter)
{
  if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        String url = "https://script.google.com/macros/s/"+ GOOGLE_SCRIPT_ID + "/exec?cell_location2=E"+String(unique_id)+"&transaction_history="+String(parameter)+"&command=update_summary";
        Serial.println("Making a request");
        http.begin(url.c_str()); //Specify the URL and certificate
        http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
        int httpCode = http.GET();
        Serial.print("HTTP status code");
        Serial.println(httpCode);
        http.end();
        
  }


}
String receive_from_user(){
  int numNewMessages=bot.getUpdates(bot.last_message_received+1);
  while(numNewMessages<1){
    numNewMessages=bot.getUpdates(bot.last_message_received+1);
  }
  String retVal=bot.messages[0].text;
  return retVal;
}
// Handle what happens when you receive new messages

int telegram_writing(String text){
    String Buffer;
    /*
    String chat_id = String(bot.messages[0].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      return 0;
    }*/
    String chat_id=CHAT_ID;
    if (text == "start") { 
      String Starting_Line="Welcome To ATM Machine Interface 🏧\n Please Select the option you want to proceed  \n Press \"1\" for Create a account ✅\n Press \"2\" to Withdrawl 💰\n Press \"3\" to Credit 💳\n Press \"4\" Balance Check ⚖️\n Press \"5\" to Check transaction history 📑"; 
      bot.sendMessage(chat_id,Starting_Line,"");
      Serial.println(Starting_Line);
      Buffer=receive_from_user();      
      String First_comm=Buffer;/*
      if(First_comm!="1" || First_comm!="2"||First_comm!="3"||First_comm!="4")
      {
        bot.sendMessage(chat_id, "Wrong Command Please Enter 'start' to begin the process!!!!", "");
        return 0;
      }
      */
      if (First_comm == "1") {
          bot.sendMessage(chat_id, "Please enter a User_name✅ ", "");
          Serial.println("Please enter a User_name ");
          String Buffer1=receive_from_user();
      
          First_comm="Your User_name is "+Buffer1;
          
          bot.sendMessage(chat_id,First_comm, "");
          bot.sendMessage(chat_id, "Please enter a Password ✅", "");
          Serial.println("Please enter a Password ");
          String Buffer2=receive_from_user();      
          First_comm="Your Password is "+Buffer2;          
          bot.sendMessage(chat_id,First_comm, "");                  
          String uni_best=write_into_Spreadsheet(Buffer1,Buffer2);
          uni_best="Your Unique_ID is :"+uni_best;
          bot.sendMessage(chat_id,uni_best, "");    
          bot.sendMessage(chat_id,"Account Created Successfully😇😇😇!!!" , ""); 
          bot.sendMessage(chat_id,"Atm Window Closing Thank You for giving your time😇😇😇!!!" , ""); 
          return 0;   
          
        }   
      
    else if(First_comm=="2")
    {
          bot.sendMessage(chat_id,"Please enter your unique ID ✅" , "");  
          Buffer=receive_from_user();  
          String unique_code=read_data(First_comm,"unique_id" );              
          First_comm=Buffer.toInt();
          while(1) 
          {
              String Buffer=receive_from_user();
              String temp=Buffer;
              
              if(unique_code!=temp)
              {
                bot.sendMessage(chat_id,"Unique Id is Incorrect!!!🥲🥲🥲 " , "");
                bot.sendMessage(chat_id,"Thank You for the Transaction ..... Please Try again😥😥😥!!! " , "");

                return 0;
              }
              else
              {
                bot.sendMessage(chat_id,"Unique Id Verified Successfully😇😇😇!!! " , ""); 
                delay(1000);
                break;
              }
        

          }
          bot.sendMessage(chat_id,"Please enter your Password  ✅!!! " , ""); 
         String pass_code=read_data(First_comm,"password" );  
          
          while(1) 
          {
              Buffer=receive_from_user();
              String temp=Buffer;
              if(pass_code!=temp)
              {
                bot.sendMessage(chat_id,"Password is Incorrect!!!🥲🥲🥲 " , "");
                bot.sendMessage(chat_id,"Thank You for the Transaction ..... Please Try again😥😥😥!!! " , "");

                return 0;
              }
              else
              {
                bot.sendMessage(chat_id,"Password Verified Successfully😇😇😇!!! " , ""); 
                delay(1000);
                break;
              }
        

          }
          bot.sendMessage(chat_id,"Please Enter Amount for Withdrawl🤑🤑!!! " , ""); 
          
          String no_name=read_data(First_comm,"available_balance");
          int read_amount=no_name.toInt();
          Buffer=receive_from_user();
          String read_history=read_data(First_comm,"transaction history");
         
          read_history=read_history+"MoneyWithdrawn:"+Buffer;
          get_update_Summary(First_comm,read_history);
          int Amount=Buffer.toInt();
          if(Amount >read_amount)
          {
            bot.sendMessage(chat_id,"Insufficient Amount 🤒🤒🤒" , ""); 
          }
          else{
            Amount=read_amount-Amount;
            update(First_comm,String(Amount));
            delay(1000);
            String balance_remain="The remaining balance is💰: "+String(Amount);           
            bot.sendMessage(chat_id, balance_remain , "");
          }
          bot.sendMessage(chat_id,"Atm Window Closing Thank You for giving your time😇😇😇!!!" , ""); 
          
          return 0;
    }
    else if(First_comm=="3")
    {
          bot.sendMessage(chat_id,"Please enter your unique ID✅ " , "");  
          Buffer=receive_from_user();  
          String unique_code=read_data(First_comm,"unique_id" );             
          First_comm=Buffer;
          while(1) 
          {
              Buffer=receive_from_user();
              String temp=Buffer;
              if(unique_code!=temp)
              {
                bot.sendMessage(chat_id,"Unique Id is Incorrect!!!🥲🥲🥲 " , "");
                bot.sendMessage(chat_id,"Thank You for the Transaction ..... Please Try again😥😥😥!!! " , "");

                return 0;
              }
              else
              {
                bot.sendMessage(chat_id,"Unique Id Verified Successfully😇😇😇!!! " , ""); 
                delay(1000);
                break;
              }
        

          }
          bot.sendMessage(chat_id,"Please enter your Password✅!!! " , ""); 
          String pass_code=read_data(First_comm,"password"); 
          Serial.print("Your passcode is: ") ;
          Serial.println(pass_code);
          
          while(1) 
          {
            Buffer=receive_from_user();
            String temp=Buffer;
          if(pass_code!=temp)
          {
              bot.sendMessage(chat_id,"Password is Incorrect!!!🥲🥲🥲 " , "");
              bot.sendMessage(chat_id,"Thank You for the Transaction ..... Please Try again😥😥😥!!! " , "");

              return 0;
          }
          else
          {
              bot.sendMessage(chat_id,"Password Verified Successfully✅✅!!! " , ""); 
              delay(1000);
              break;
          }
          
          }
          bot.sendMessage(chat_id,"Please Enter Amount for Crediting💳💳!!! " , ""); 
          
          String no_name=read_data(First_comm,"available_balance");
          int read_amount=no_name.toInt();
          Buffer=receive_from_user();
          String read_history=read_data(First_comm,"transaction history");
          read_history=read_history+"MoneyCreadited:"+Buffer;
          get_update_Summary(First_comm,read_history);
          
          int Amount=Buffer.toInt();
          Amount=read_amount+Amount;
          update(First_comm,String(Amount));
          delay(1000);
          String balance_remain=String(Amount);
          balance_remain="The remaining balance is 💰: "+balance_remain;
          bot.sendMessage(chat_id, balance_remain , "");         
          bot.sendMessage(chat_id,"Atm Window Closing Thank You for giving your time😇😇😇!!!" , ""); 
          delay(100);
          return 0;
    }
    else if(First_comm=="4")
    {
      bot.sendMessage(chat_id,"Please enter your unique_id✅:", "");
      First_comm=receive_from_user();
      String read_amount=read_data(First_comm,"available_balance");
      read_amount="The available balance is💰: "+read_amount;
      bot.sendMessage(chat_id, read_amount, "");
      bot.sendMessage(chat_id,"Atm Window Closing Thank You for giving your time😇😇😇!!!" , ""); 
      return 0;  
      

    }
    else if(First_comm=="5")
    {
      bot.sendMessage(chat_id,"Please enter your unique_id✅:", "");
      First_comm=receive_from_user();
      String read_amount=read_data(First_comm,"transaction_history");
    
      bot.sendMessage(chat_id, read_amount, "");
      bot.sendMessage(chat_id,"Atm Window Closing Thank You for giving your time😇😇😇!!!" , ""); 
      return 0;

    }

      else
    {
      bot.sendMessage(chat_id,"Please enter 'start' to write the operation😥😥😥" , "");
      return 0;
    }   
 }   
}
void setup() {
  Serial.begin(115200);

  
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);
  
  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  Serial.println("Connecting to WiFi..");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  // Print ESP32 Local IP Address
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  Serial.println(WiFi.localIP());
}

// int numNewmsg()
// {
// int numNewMessages;
// if (millis() > lastTimeBotRan + botRequestDelay)  {
//     int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

//     while(numNewMessages) {
//       Serial.println("got response");
//       numNewMessages = bot.getUpdates(bot.last_message_received + 1);
//     }
//     lastTimeBotRan = millis();
//   }
// return numNewMessages;
// }


void loop() {
  // Vector<String> udemy;
  // int numNewMessages=0;
  // numNewMessages=numNewmsg();
  // udemy=handleNewMessages(numNewMessages);
  // text=udemy[1];
  // numNewMessages=udemy[0].toInt();
  String text=receive_from_user();
  telegram_writing(text);

  
}
