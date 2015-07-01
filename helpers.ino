void getAngle()
{
  char c;
  while(Serial.available())
  {
    delay(10);
    c = Serial.read();
    buf += c;
    Serial << c;
  }

  if(buf.length() == 0)
    return;

  if(buf.charAt(0) == 'Q' || buf.charAt(0) == 'q')
  {
    Serial << endl << "Quitting to main." << endl;
    buf="";
    state = 0;
    return;
  }

  Serial << endl;
  char conv[buf.length()];
  buf.toCharArray(conv,buf.length()+1);
  double angle = atof(conv);
  double dTmp = stepsPerRevolution * ( angle/ ((double) 360));
  stepsToDrive = round(dTmp);
  Serial << "stepsPerRevolution: " << stepsPerRevolution << endl;
  Serial << "Driving " << stepsToDrive << "/" << dTmp <<" steps for angle " << angle << endl;
  state = 10;
  buf="";
  return;
}

bool getCycleStart()
{
  char c;
  while(Serial.available())
  {
    delay(10);
    c = Serial.read();
    Serial << c;
    buf += c;
  }

  if(buf.length() == 0)
    return false;

  if(buf.charAt(0) =='!')
  {
    buf = "";
    state = 4;
    return true;
  }

  return false;
}

void getDivisions()
{
  char c;
  while(Serial.available())
  {
    delay(10);
    c = Serial.read();
    Serial << c;
    buf += c;
  }

  if(buf.length() == 0)
    return;

  if(buf.charAt(0) == 'Q' or buf.charAt(0) == 'q')
  {
    Serial << endl << "Quitting to main." << endl;
    buf="";
    state = 0;
    return;
  }

  totalDivisions = (unsigned int) buf.toInt();
  Serial << endl;

  buf="";
  return;
}

void getMode()
{
  char c;
  while(Serial.available())
  {
    delay(10);
    c = Serial.read();
    Serial << c;
    buf += c;
  }

  if(buf.length() == 0)
    return;

  if(buf.length() >= 3)
  {
    Serial << endl << "getMode(): buf length >=2. exiting!" << endl;
    state = 0;
    return;
  }

  c = buf.charAt(0);

  if(c == 'D' || c == 'd')
  {
    Serial << endl << "Entering Division mode." << endl;
    buf="";
    state = 2;
    return;
  }

  if(c == 'A' || c == 'a')
  {
    Serial << endl << "Entering Angle mode." << endl;
    buf="";
    state = 8;
    return;
  }

  if(c == 'Q' || c == 'q')
  {
    Serial << endl << "Quitting to main." << endl;
    buf="";
    state = 0;
    return;
  }

  Serial << endl << "Error. Please choose either D / A / Q." << endl;
  state = 0;
  return;
}

void rotateSteps()
{
  for(unsigned long i = 0; i < stepsToDrive; i++)
  {
    digitalWrite(pinStep, invertStep^true);
    delayMicroseconds(stepPeriod);
    digitalWrite(pinStep, invertStep^false);
    delayMicroseconds(stepPeriod);
  }
  Serial << "[ DONE ]" << endl;
}
