function doGet(e){
    // Gets the cardID from the ESP32
    let cardID = e.parameter.cardID;

    // Gets the active spreadsheet and the sheets that are needed
    let spreadSheet = SpreadsheetApp.getActiveSpreadsheet();
    let timeSheet = spreadSheet.getSheetByName("Time Tracker");
    let logsSheet = spreadSheet.getSheetByName("Logs");
    let sessionsSheet = spreadSheet.getSheetByName("Sessions");

    // Gets the current time/date
    let time = new Date();

    // Gets the current users in the time tracker sheet
    // users is a 2D array, where the outer array is the rows and the inner arrays are the columns of the sheet.
    let users = timeSheet.getDataRange().getValues();

    let name = "";
    let found = false;

    // Loops through the rows to check if a card ID already exists in the time tracker sheet.
    
    for(let i = 1; i < users.length; i++){
        // Get the name of the user if the card ID is found
        if(users[i][0] == cardID){
            name = users[i][0] || "Unknown";
            found = true;
            break;
        }
    }

    // If the card ID is not found, add it to the time tracker sheet with the current time and date
    if(!found){
        name = "Unknown";
        timeSheet.appendRow([cardID, time]);
    }