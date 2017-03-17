#include "FS.h"

int current_total_lines = 0;
int last_index = 0;

const int MAX_INDEX_LINES = 20;

void setup() {
  Serial.begin(115200);
  SPIFFS.begin();
  
  // Serial.println("Please wait 30 secs for SPIFFS to be formatted");
  // SPIFFS.format();
  // Serial.println("Spiffs formatted");

  read_info();

  Serial.print("Total lines : ");
  Serial.println(current_total_lines);

  Serial.print("Last Index : ");
  Serial.println(last_index);

  write_test_data();
  delay(2000);
  read_test_data();

}

//
// Read total lines
// Read last max index number in log file
// <index> - <method-name>
void read_info() {

  File file = SPIFFS.open("/f.txt", "r");
  if (!file) {
    Serial.println("file open failed");
  }

  while (true) {
    String s = file.readStringUntil('\n');
    if (strlen(s.c_str()) > 0) {
      // Add total lines counted
      current_total_lines++;

      // Read the last max line number from the log file
      long read_line_no = strtol (s.c_str(), NULL, 10);
      if (last_index < read_line_no) {
        last_index = read_line_no;
      }
    } else {
      break;
    }
  }

  file.close();
}

void write_test_data() {

  File file = SPIFFS.open("/f.txt", "w");
  if (!file) {
    Serial.println("File open failed for writing.");
  }

  Serial.println("Write test data");

  int current_index = 1;

  for (int index = 0; index <= 28; index++) {
    file.printf("%2d - %8s\n", index, "method()");

    if (current_index < MAX_INDEX_LINES) {
      current_index++;
    } else {
      current_index = 0;
      file.seek(0, SeekSet);

      Serial.print("File position reset : " );
      Serial.println(file.position());
    }
  }

  file.close();
}

void read_test_data() {

  File file = SPIFFS.open("/f.txt", "r");
  if (!file) {
    Serial.println("File open failed for reading.");
  }
  
  Serial.println("Reading file");

  while (true) {
    String s = file.readStringUntil('\n');
    if (strlen(s.c_str()) < 1) { // EOF
      break;
    }
    Serial.println(s);
  }

  file.close();
}

void loop() {

}

