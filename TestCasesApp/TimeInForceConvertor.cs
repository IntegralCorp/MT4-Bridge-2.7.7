using FileHelpers;
using System;

namespace TestCasesApp
{
  public class TimeInForceConvertor : ConverterBase
  {
    public override object StringToField(string from)
    {
      char result;
      switch (from)
      {
        case "FOK":
          result = '4';
          break;
        case "IOC":
          result = '3';
          break;
        default:
          result = '6';
          break;
      }
      //---
      return result;
    }

    public override string FieldToString(object fieldValue)
    {
      string result = "";
      switch ((char)fieldValue)
      {
        case '4':
          result = "FOK";
          break;
        case '3':
          result = "IOC";
          break;
        default:
          result ="GTD";
          break;
      }
      //---
      return result;
    }
  }
}
