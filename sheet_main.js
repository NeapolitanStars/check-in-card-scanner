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
    
    // Gets the logs from the logs sheet
    let logs = logsSheet.getDataRange().getValues();

    // Loops through the logs to find the last action of the card ID.
    let lastAction = "CHECKED OUT";
    for (let i = logs.length - 1; i > 0; i--){
        // If the card ID is found, get the last action
        if(logs[i][0] == cardID){
            lastAction = logs[i][3] || "CHECKED OUT";
            break;
        }
    }

    // Assigns the "current" action variable based on the opposite of the last action.
    let action;
    if (lastAction == "CHECKED OUT"){
        action = "CHECKED IN";
    } else {
        action = "CHECKED OUT";
    }

    logsSheet.appendRow([cardID, name, time, action]);

    if (action == "CHECKED IN"){
        sessionsSheet.appendRow([cardID, name, time, "", ""]);
    } else {
        // Gets the sessions from the sessions sheet
        let sessions = sessionsSheet.getDataRange().getValues();
    } 

    // Loops through the sessions to find the last session of the card ID and updates the check out time.
    for (let i = sessions.length - 1; i > 0; i--){
        if(sessions[i][0] == cardID && sessions[i][3] == ""){
            sessionsSheet.getRange(i + 1, 4).setValue(time);

            // Calculate the amount of hours between the check in time and the check out time and updates the hours column.
            let checkInTime = sessions[i][2];
            let hours = (time - checkInTime) / 3600000; // Convert milliseconds to hours
            sessionsSheet.getRange(i + 1, 5).setValue(hours);
        }
    }
    return ContentService.createTextOutput(action);
}  