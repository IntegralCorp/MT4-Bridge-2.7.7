using FileHelpers;
using System;

namespace TestCasesApp
{
  public class DateConvertor : ConverterBase
  {
    public override object StringToField(string from)
    {
      if (string.IsNullOrEmpty(from))
      {
        return (new DateTime(1970, 1, 1, 0, 0, 0));
      }
      else
      {
        from = from.Replace("-", " ").Replace(".", "");
        if (from.Contains(":"))
          return (DateTime.ParseExact(from, "yyyyMMdd HH:mm:ss", MyCulture.Culture));
        else
        {
          return long.Parse(from).ConvertFromUnixTime();
        }
      }
    }

    public override string FieldToString(object fieldValue)
    {
      if (((DateTime)fieldValue).Year.Equals(1970) || ((DateTime)fieldValue).Year.Equals(1))
      {
        return ("");
      }
      //---
      return ((DateTime)fieldValue).ToString("yyyyMMdd HH:mm:ss");
    }
  }
}
