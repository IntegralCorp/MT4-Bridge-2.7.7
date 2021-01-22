using FileHelpers;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestCasesApp
{
  public class BooleanConvertor : ConverterBase
  {    public override object StringToField(string from)
    {
      return from.Trim()=="Bid";
    }

    public override string FieldToString(object fieldValue)
    {
      return (bool)fieldValue?" Bid":" Ask";
    }
  }
}
