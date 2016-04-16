// See README.txt for information and build instructions.

#include <iostream>
#include <fstream>
#include <string>
//#include "CPlusPlusCode/addressbook.pb.h"
#include "CPlusPlusCode/ProtoBufMsg.pb.h"
using namespace std;

//// This function fills in a Person message based on user input.
//void PromptForAddress(tutorial::Person* person) {
//  cout << "Enter person ID number: ";
//  int id;
//  cin >> id;
//  person->set_id(id);
//  cin.ignore(256, '\n');
//
//  cout << "Enter name: ";
//  getline(cin, *person->mutable_name());
//
//  cout << "Enter email address (blank for none): ";
//  string email;
//  getline(cin, email);
//  if (!email.empty()) {
//    person->set_email(email);
//  }
//
//  while (true) {
//    cout << "Enter a phone number (or leave blank to finish): ";
//    string number;
//    getline(cin, number);
//    if (number.empty()) {
//      break;
//    }
//
//    tutorial::Person::PhoneNumber* phone_number = person->add_phones();
//    phone_number->set_number(number);
//
//    cout << "Is this a mobile, home, or work phone? ";
//    string type;
//    getline(cin, type);
//    if (type == "mobile") {
//      phone_number->set_type(tutorial::Person::MOBILE);
//    } else if (type == "home") {
//      phone_number->set_type(tutorial::Person::HOME);
//    } else if (type == "work") {
//      phone_number->set_type(tutorial::Person::WORK);
//    } else {
//      cout << "Unknown phone type.  Using default." << endl;
//    }
//  }
//}

// Main function:  Reads the entire address book from a file,
//   adds one person based on user input, then writes it back out to the same
//   file.
int main(int argc, char* argv[]) {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  MarketUpdate mktUpdt;
  mktUpdt.set_code("000001");
  mktUpdt.set_symbol("test");
  mktUpdt.set_exchange(SHSE);
  mktUpdt.set_exchange_timestamp("112233444");

  std::string mktStr;
  mktUpdt.SerializeToString(&mktStr);
  //std::cout << mktStr;

  MessageBase msgBase;
  msgBase.set_type(TYPE_MARKETUPDATE);
  msgBase.set_msg(mktStr);
  std::string msgStr;
  msgBase.SerializeToString(&msgStr);
  //std::cout << msgStr;

  MessageBase tmp;
  tmp.ParseFromString(msgStr);
  std::cout << tmp.type() << std::endl;

  MarketUpdate mktTmp;
  mktTmp.ParseFromString(tmp.msg());
  std::cout << mktTmp.code() << std::endl;
  std::cout << mktTmp.symbol() << std::endl;
  std::cout << mktTmp.exchange() << std::endl;

  //if (argc != 2) {
  //  cerr << "Usage:  " << argv[0] << " ADDRESS_BOOK_FILE" << endl;
  //  return -1;
  //}

  //tutorial::AddressBook address_book;

  //{
  //  // Read the existing address book.
  //  fstream input(argv[1], ios::in | ios::binary);
  //  if (!input) {
  //    cout << argv[1] << ": File not found.  Creating a new file." << endl;
  //  } else if (!address_book.ParseFromIstream(&input)) {
  //    cerr << "Failed to parse address book." << endl;
  //    return -1;
  //  }
  //}

  //// Add an address.
  //PromptForAddress(address_book.add_people());

  //{
  //  // Write the new address book back to disk.
  //  fstream output(argv[1], ios::out | ios::trunc | ios::binary);
  //  if (!address_book.SerializeToOstream(&output)) {
  //    cerr << "Failed to write address book." << endl;
  //    return -1;
  //  }
  //}

  // Optional:  Delete all global objects allocated by libprotobuf.
  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}
