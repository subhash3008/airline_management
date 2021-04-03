#include <iostream>
#include <iomanip>
#include <string>
#include <string>
#include "mysql.h"

// Global Variable
int qstate;
MYSQL* conn;
MYSQL_ROW row;
MYSQL_RES* res;
//

void connectToAirlineDb();

// reservation functions
void displayMainMenuOptions();
int getMainMenuResponse();
void printReservationHeader();
void printReservation();
void reserveSeat();
void userTicketHistory();
void displayPassenger();

// flight details functions
void displayFlightMenuOptions();
int getFlightMenuResponse();
void printFlightHeader();
void printFlightRow();
void addNewFlight(int);
void listFlight();
void editFlight();
void deleteFlight();
void reactivateFlight();
void flightDetails();

void connectAirlinesDb() {
  conn = mysql_init(0);
  if (conn) {
    std::cout << "Database Connected!!" << std::endl;
    std::cout << "Press any key to continue..." << std::endl;
    system("cls");
  } else {
    std::cout << "Failed to connect.." << mysql_errno(conn) << std::endl;
  }
  conn = mysql_real_connect(conn, "localhost", "root", "password_for_root", "cpp_airline_reservation_db", 3306, NULL, 0);
  if (conn) {
    std::cout << "Database connected to Mysql.." << conn << std::endl;
    std::cout << "Press any key to continue.." << std::endl;
    system("cls");
  } else {
    std::cout << "Failed to connect" << mysql_errno(conn) << std::endl;
  }
};

int main() {
  // Initial Load
  system("cls");
  system("title Airline Reservation System Program");
  system("color 0f");

  connectAirlinesDb();

  int menuSelection{ 0 };

  while (true) {
    displayMainMenuOptions();
    menuSelection = getMainMenuResponse();

    switch (menuSelection) {
      case 1: reserveSeat(); break;
      case 2: userTicketHistory(); break;
      case 3: listFlight(); break;
      case 4: displayPassenger(); break;
      case 5: flightDetails(); break;
      case 6: exit(0);
    }
  }
  return 0;
}

void displayMainMenuOptions() {
  std::cout << "****************************************" << std::endl;
  std::cout << "Welcome to airlines reservation system!!" << std::endl;
  std::cout << "Airline Reservation System Menu" << std::endl;
  std::cout << "1. Reserve Seat" << std::endl;
  std::cout << "2. User Tickets" << std::endl;
  std::cout << "3. Flights Schedule" << std::endl;
  std::cout << "4. Display Passenger" << std::endl;
  std::cout << "5. Flight Details" << std::endl;
  std::cout << "6. Exit Program" << std::endl;
  std::cout << "****************************************" << std::endl;
}

int getMainMenuResponse() {
  int response;
  while (true) {
    std::cout << "Please choose from above menu options." << std::endl;
    std::cin >> response;
    if (response > 0 && response < 7) {
      return response;
    }
  }
}

void printReservationHeader() {
  int width = 16;
  std::cout << std::left << std::setw(width) << std::setfill(' ') << "Ticket ID";
  std::cout << std::left << std::setw(width) << std::setfill(' ') << "User Name";
  std::cout << std::left << std::setw(width) << std::setfill(' ') << "Passport";
  std::cout << std::left << std::setw(width) << std::setfill(' ') << "Contact Number";
  std::cout << std::left << std::setw(width) << std::setfill(' ') << "Flight Number";
  std::cout << std::left << std::setw(width) << std::setfill(' ') << "Flight Date";
  std::cout << std::left << std::setw(width) << std::setfill(' ') << "User Address";
  std::cout << std::endl;
}

void printReservation() {
  int columnCount = mysql_num_fields(res);
  for (int i = 0; i < columnCount; ++i) {
    std::cout << std::left << std::setw(16) << std::setfill(' ') << row[i];
  }
  std::cout << std::endl;
}

void reserveSeat() {
  std::string userName;
  std::string userPassportNo;
  std::string userContactNumber;
  std::string flightNo;
  std::string flightDate;
  std::string userAddress;

  std::cin.ignore();
  std::cout << "Enter User Name : ";
  std::getline(std::cin, userName);
  std::cout << "Enter Passport Number : ";
  std::getline(std::cin, userPassportNo);
  std::cout << "Enter Contact Number : ";
  std::getline(std::cin, userContactNumber);
  std::cout << "Enter User Address : ";
  std::getline(std::cin, userAddress);

  std::cout << std::endl;
  listFlight();
  std::cout << std::endl;

  std::cout << "Enter Flight Number : ";
  std::getline(std::cin, flightNo);
  std::cout << "Enter Date of Journey (YYYY-MM-DD) : ";
  std::getline(std::cin, flightDate);

  std::string query = "INSERT INTO reservation_tb (r_username, r_passport_number, r_contact_number, r_flight_number, r_flight_date, r_user_address) VALUES ('";
  query += userName + "', '" + userPassportNo + "', '" + userContactNumber + "', '" + flightNo + "', '" + flightDate + "', '" + userAddress + "');";
  const char* q = query.c_str();
  qstate = mysql_query(conn, q);
  if (!qstate) {
    std::cout << "Booking confirmed.." << std::endl;
  } else {
    std::cout << "Error with database" << mysql_errno(conn) << std::endl;
  }
}

void userTicketHistory() {
  std::string userName;
  std::cin.ignore();
  std::cout << "Enter user name : ";
  std::getline(std::cin, userName);

  std::string q = "SELECT * FROM reservation_tb WHERE r_username='" + userName + "';";
  const char* c_q = q.c_str();
  qstate = mysql_query(conn, c_q);
  if (!qstate) {
    res = mysql_store_result(conn);
    auto rowCount = mysql_num_rows(res);
    if (rowCount == 0) {
      std::cout << "No data found for entered user." << std::endl;
      return;
    }
    printReservationHeader();
    while (row = mysql_fetch_row(res)) {
      printReservation();
    }
  } else {
    std::cout << "Error with Database" << mysql_errno(conn) << std::endl;
  }
}

void displayPassenger() {
  std::string flightNo;
  std::string flightDate;
  std::cin.ignore();
  std::cout << "Enter Flight Number : ";
  std::getline(std::cin, flightNo);
  std::cout << "Enter Date of Journey (YYYY-MM-DD) : ";
  std::getline(std::cin, flightDate);
  
  std::string q = "SELECT * FROM reservation_tb WHERE r_flight_number='" + flightNo + "' AND r_flight_date='" + flightDate + "';";
  const char* c_q = q.c_str();
  qstate = mysql_query(conn, c_q);
  if (!qstate) {
    res = mysql_store_result(conn);
    auto rowCount = mysql_num_rows(res);
    if (rowCount == 0) {
      std::cout << "No flight details found with provided input." << std::endl;
      return;
    }
    printReservationHeader();
    while (row = mysql_fetch_row(res)) {
      printReservation();
    }
  } else {
    std::cout << "Error with database" << mysql_errno(conn) << std::endl;
  }
}


/////////////////// FLIGHT DETAILS /////////////////////
void displayFlightMenuOptions() {
  std::cout << "*******************************" << std::endl;
  std::cout << "Airlines Flight Management Menu" << std::endl;
  std::cout << "1. Add Flight." << std::endl;
  std::cout << "2. Edit Flight." << std::endl;
  std::cout << "3. Delete Flight." << std::endl;
  std::cout << "4. Reactivate Flight." << std::endl;
  std::cout << "5. List Flights." << std::endl;
  std::cout << "6. Back To Menu." << std::endl;
  std::cout << "*******************************" << std::endl;
}

int getFlightMenuResponse() {
  int selection;
  while (true) {
    std::cout << "Choose an option from menu above : ";
    std::cin >> selection;
    if (selection > 0 && selection < 7) {
      return selection;
    } else {
      std::cout << "Please select valid option!!" << std::endl;
    }
  }
}

void printFlightHeader() {
  std::cout << std::left << std::setw(15) << std::setfill(' ') << "Flight No.";
  std::cout << std::left << std::setw(15) << std::setfill(' ') << "Flight Name";
  std::cout << std::left << std::setw(15) << std::setfill(' ') << "Depart From";
  std::cout << std::left << std::setw(15) << std::setfill(' ') << "Arrive To";
  std::cout << std::left << std::setw(15) << std::setfill(' ') << "Depart Time";
  std::cout << std::left << std::setw(15) << std::setfill(' ') << "Booking Amount";
  std::cout << std::endl;
}

void printFlightRow() {
  for (int i = 1; i < 7; ++i) {
    std::cout << std::left << std::setw(15) << std::setfill(' ') << row[i];
  }
  std::cout << std::endl;
}

void addNewFlight(int updateRowId = -1) {
  std::string flightNo;
  std::string flightName;
  std::string flightFrom;
  std::string flightDestination;
  std::string flightTime;
  std::string flightAmount;
  std::string flightAvaialability;
  std::string q;

  std::cin.ignore();

  if (updateRowId != -1) {
    std::cout << "Please provide below details to update the flight in the system : " << std::endl;
  } else {
    std::cout << "Please provide below details to add a new flight in the system : " << std::endl;
  }
  std::cout << "Enter Flight Number : ";
  std::getline(std::cin, flightNo);
  std::cout << "Enter Flight Name : ";
  std::getline(std::cin, flightName);
  std::cout << "Enter Flight Departure Location : ";
  std::getline(std::cin, flightFrom);
  std::cout << "Enter Flight Arrival Location : ";
  std::getline(std::cin, flightDestination);
  std::cout << "Enter Flight Time : ";
  std::getline(std::cin, flightTime);
  std::cout << "Enter Flight Amount : ";
  std::getline(std::cin, flightAmount);
  if (updateRowId != -1) {
    std::cout << "Enter Flight Avaialability (A (Available) / U (Unavailable)) : ";
    std::getline(std::cin, flightAvaialability);
  }

  if (updateRowId != -1) {
    q = "UPDATE flightdetails_tb SET ";
    q += "f_no='" + flightNo + "', ";
    q += "f_name = '" + flightName + "', ";
    q += "f_from = '" + flightFrom + "', ";
    q += "f_destination='" + flightDestination + "', ";
    q += "f_time='" + flightTime + "', ";
    q += "f_amount='" + flightAmount + "', ";
    q += "f_available='" + flightAvaialability + "' ";
    q += "WHERE f_id=" + std::to_string(updateRowId) + ";";
  } else {
    q = "insert into flightdetails_tb (f_no, f_name, f_from, f_destination, f_time, f_amount, f_available) values('" + flightNo + "','" + flightName + "','" + flightFrom + "','" + flightDestination + "','" + flightTime + "','" + flightAmount + "','A')";
  }
  const char* c_q = q.c_str();
  qstate = mysql_query(conn, c_q);
  if (!qstate) {
    std::cout << "SuccessFully updated the database." << std::endl;
  } else {
    std::cout << "Query Execution Problem " << mysql_errno(conn) << std::endl;
  }
}

void listFlight() {
  std::string query = "SELECT * FROM flightdetails_tb where f_available='A';";
  const char* q = query.c_str();
  qstate = mysql_query(conn, q);
  if (!qstate) {
    res = mysql_store_result(conn);
    printFlightHeader();
    while (row = mysql_fetch_row(res)) {
      printFlightRow();
    }
  } else {
    std::cout << "Query Failed : " << mysql_error(conn) << std::endl;
  }
}

void editFlight() {
  std::cin.ignore();
  int updateRowId{ -1 };
  std::string flightNumber;
  std::cout << "Please enter flight number to edit : ";
  std::getline(std::cin, flightNumber);
  std::string query = "SELECT * FROM flightdetails_tb WHERE f_no='" + flightNumber + "'";
  const char* q = query.c_str();
  qstate = mysql_query(conn, q);
  if (!qstate) {
    res = mysql_store_result(conn);
    auto rowCount = mysql_num_rows(res);
    if (rowCount == 0) {
      std::cout << "No flight with given flight number. Please try again.." << std::endl;
      return;
    }
    printFlightHeader();
    while (row = mysql_fetch_row(res)) {
      printFlightRow();
      updateRowId = std::stoi(row[0]);
    }
    char editConfirm;
    while (true) {
      std::cout << "Please enter Y/y to confirm editing or N/n to return to flight details menu : ";
      std::cin >> editConfirm;
      if (editConfirm == 'Y' || editConfirm == 'y' || editConfirm == 'N' || editConfirm == 'n') {
        break;
      }
    }
    if (editConfirm == 'Y' || editConfirm == 'y') {
      addNewFlight(updateRowId);
    }
  } else {
    std::cout << "Cannot execute query" << mysql_errno(conn) << std::endl;
  }
}

void deleteFlight() {
  std::cin.ignore();
  int flightId{ -1 };
  std::string flightNo;
  std::cout << "Enter the flight no. to delete the flight : ";
  std::getline(std::cin, flightNo);
  std::string query = "SELECT * FROM flightdetails_tb WHERE f_no='" + flightNo + "';";
  const char* q = query.c_str();
  qstate = mysql_query(conn, q);
  if (!qstate) {
    res = mysql_store_result(conn);
    auto rowCount = mysql_num_rows(res);
    if (rowCount == 0) {
      std::cout << "No flight with given flight number. Please try again.." << std::endl;
      return;
    }
    printFlightHeader();
    while (row = mysql_fetch_row(res)) {
      printFlightRow();
      flightId = std::stoi(row[0]);
    }
    char deleteConfirm;
    while (true) {
      std::cout << "Enter Y/y to confirm delete or N/n to return to flight menu : ";
      std::cin >> deleteConfirm;
      if (std::toupper(deleteConfirm) == 'Y' || std::toupper(deleteConfirm) == 'N') {
        break;
      }
    }
    if (std::toupper(deleteConfirm) == 'Y') {
      std::string delQuery = "UPDATE flightdetails_tb SET f_available='U' where f_id=" + std::to_string(flightId) + ";";
      const char* delQ = delQuery.c_str();
      qstate = mysql_query(conn, delQ);
      if (!qstate) {
        std::cout << "Row successfully deleted!!" << std::endl;
      } else {
        std::cout << "Error deleting flight. " << mysql_errno(conn) << std::endl;
      }
    }

  } else {
    std::cout << "Error with database" << mysql_errno(conn) << std::endl;
  }
}

void reactivateFlight() {
  std::string flightNo;
  int flightId{ -1 };
  std::cin.ignore();
  std::cout << "Please enter the flight number to resume the flight service : ";
  std::getline(std::cin, flightNo);
  std::string query = "SELECT * FROM flightdetails_tb WHERE f_no='" + flightNo + "' AND f_available='U';";
  const char* q = query.c_str();
  qstate = mysql_query(conn, q);
  if (!qstate) {
    res = mysql_store_result(conn);
    auto rowCount = mysql_num_rows(res);
    if (rowCount == 0) {
      std::cout << "No flight found with given number. Please try again.." << std::endl;
      return;
    }
    printFlightHeader();
    while (row = mysql_fetch_row(res)) {
      printFlightRow();
      flightId = std::stoi(row[0]);
    }
    char resumeConfirm;
    while (true) {
      std::cout << "Please enter Y/y to confirm flight service resume or N/n to return to flight menu : ";
      std::cin >> resumeConfirm;
      if (std::toupper(resumeConfirm) == 'Y' || std::toupper(resumeConfirm) == 'N') {
        break;
      }
    }
    if (std::toupper(resumeConfirm) == 'Y') {
      std::string resumeQuery = "UPDATE flightdetails_tb SET f_available='A' where f_id=" + std::to_string(flightId) + ";";
      const char* resumeQ = resumeQuery.c_str();
      qstate = mysql_query(conn, resumeQ);
      if (!qstate) {
        std::cout << "Flight service for chosen flight resumed successfully.." << std::endl;
      } else {
        std::cout << "Error with database" << mysql_errno(conn) << std::endl;
      }
    }
  } else {
    std::cout << "Error with database" << mysql_errno(conn) << std::endl;
  }
}

void flightDetails() {
  system("cls");
  while (true) {
    displayFlightMenuOptions();
    int flightMenuSelection{ getFlightMenuResponse() };

    switch (flightMenuSelection) {
      case 1: addNewFlight(); break;
      case 2: editFlight(); break;
      case 3: deleteFlight(); break;
      case 4: reactivateFlight(); break;
      case 5: listFlight(); break;
      case 6: return;
    }
  }
}