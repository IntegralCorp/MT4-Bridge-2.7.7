using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestCasesApp
{
  public class CFixField
  {
    public int Tag { get; set; }
    public string Value { get; set; }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="tag"></param>
    /// <param name="value"></param>
    public CFixField(int tag,string value)
    {
      Tag = tag;
      Value = value;
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="tag"></param>
    public CFixField(int tag)
    {
      Tag = tag;
      Value = "";
    }

    /// <summary>
    /// 
    /// </summary>
    public void Clear()
    {
      Value = "";
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="tag"></param>
    /// <param name="value"></param>
    public void SetField(int tag,string value)
    {
      if(Tag==tag)
      {
        Value = value;
      }
    }
  }
}
