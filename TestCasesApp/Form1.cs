using FileHelpers;
using QuickFix;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using TestCasesApp.Common;

namespace TestCasesApp
{
	public partial class Form1: Form
	{
		/// <summary>
		/// 
		/// </summary>
		private readonly List<COrders> records = new List<COrders>();
		private readonly BindingList<CFixMessage> messages = new BindingList<CFixMessage>();
		private readonly BindingList<CDictionary> dict = new BindingList<CDictionary>();
		private readonly BindingList<CTrade210> tradeRecords;
		private List<CTrade210> backing = new List<CTrade210>();

		private CTrade210 Selected = null;

		private static bool finished_reading = false;

		private DataTable dtMessages = new DataTable();
		private string filterField = "Message";
		/// <summary>
		/// 
		/// </summary>
		public Form1()
		{
			InitializeComponent();

			tradeRecords=new BindingList<CTrade210>(backing);
			//---
			ordersGridView.AutoGenerateColumns=false;
			messagesGridView.AutoGenerateColumns=false;
			dictGridView.AutoGenerateColumns=false;
			//---
			ordersGridView.DataSource=tradeRecords;// records;
																						 //---
			FillDictionaryTable();
			dictGridView.DataSource=dict;
		}

		private void FillDictionaryTable()
		{
			//---
			dict.Add(new CDictionary() { FieldTag="35",FieldName="MsgType",FieldValue="",FieldExpected="" });
			dict.Add(new CDictionary() { FieldTag="39",FieldName="OrdStatus",FieldValue="",FieldExpected="" });
			dict.Add(new CDictionary() { FieldTag="11",FieldName="ClOrdID",FieldValue="",FieldExpected="" });
			dict.Add(new CDictionary() { FieldTag="38",FieldName="OrdQty",FieldValue="",FieldExpected="" });
			dict.Add(new CDictionary() { FieldTag="40",FieldName="OrdType",FieldValue="",FieldExpected="" });
			dict.Add(new CDictionary() { FieldTag="31",FieldName="Price",FieldValue="",FieldExpected="" });
			dict.Add(new CDictionary() { FieldTag="44",FieldName="Order Price",FieldValue="",FieldExpected="" });
			dict.Add(new CDictionary() { FieldTag="54",FieldName="Side",FieldValue="",FieldExpected="" });
			dict.Add(new CDictionary() { FieldTag="55",FieldName="Symbol",FieldValue="",FieldExpected="" });
			dict.Add(new CDictionary() { FieldTag="59",FieldName="TimeInForce",FieldValue="",FieldExpected="" });
			dict.Add(new CDictionary() { FieldTag="14",FieldName="CumQty",FieldValue="",FieldExpected="" });
			dict.Add(new CDictionary() { FieldTag="7547",FieldName="UserDefinedSpread",FieldValue="",FieldExpected="" });
			dict.Add(new CDictionary() { FieldTag="211",FieldName="PegOffsetValue",FieldValue="",FieldExpected="" });
			dict.Add(new CDictionary() { FieldTag="7554",FieldName="AdditionalExecInst",FieldValue="",FieldExpected="" });
			dict.Add(new CDictionary() { FieldTag="151",FieldName="LeavesQty",FieldValue="",FieldExpected="" });
			//---
		}
		/// <summary>
		/// 
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void btnOrders_Click(object sender,EventArgs e)
		{
			FileHelperAsyncEngine<COrders> engine = new FileHelperAsyncEngine<COrders>();
			engine.ErrorMode=ErrorMode.SaveAndContinue;
			//engine.AfterReadRecord += AfterReadRecord;
			engine.BeforeReadRecord+=BeforeReadRecord;
			//---
			btnOrders.Enabled=false;
			finished_reading=false;
			//---
			records.Clear();
			var fileContent = string.Empty;
			var filePath = string.Empty;

			using(OpenFileDialog openFileDialog = new OpenFileDialog())
			{
				openFileDialog.InitialDirectory=folderBox.Text;
				openFileDialog.Filter="txt files (*.txt)|*.txt|All files (*.*)|*.*";
				openFileDialog.FilterIndex=2;
				openFileDialog.RestoreDirectory=true;

				if(openFileDialog.ShowDialog()==DialogResult.OK)
				{
					//Get the path of specified file
					filePath=openFileDialog.FileName;
					// Read
					using(engine.BeginReadFile(filePath))
					{
						// The engine is IEnumerable
						foreach(COrders order in engine)
						{
							// your code here
							records.Add(order);
						}
					}
				}
			}
			//---
			ReadRawReports();
			//---
			ReadFixMessages();
			//---
			btnOrders.Enabled=true;
			//---
			finished_reading=true;
		}

		private void BeforeReadRecord(EngineBase engine,FileHelpers.Events.BeforeReadEventArgs<COrders> e)
		{
			if(string.IsNullOrEmpty(e.RecordLine))
				e.SkipThisRecord=true;
		}

		private void AfterReadRecord(EngineBase engine,FileHelpers.Events.AfterReadEventArgs<COrders> e)
		{
			//throw new NotImplementedException();
		}

		/// <summary>
		/// 
		/// </summary>
		private void ReadRawReports()
		{
			tradeRecords.Clear();

			FileHelperAsyncEngine<CTrade210> engine = new FileHelperAsyncEngine<CTrade210>();
			engine.ErrorMode=ErrorMode.SaveAndContinue;
			//---
			engine.AfterReadRecord+=Engine_AfterReadRecord;
			//---
			string[] filePaths = Directory.GetFiles(folderBox.Text,
				"METATECH*.csv",
				SearchOption.TopDirectoryOnly);

			foreach(var file in filePaths)
			{
				// Read
				using(engine.BeginReadFile(file))
				{
					// The engine is IEnumerable
					foreach(CTrade210 report in engine)
					{
						var order = records.FirstOrDefault(o => o.Order == report.MainOrder || o.Order == report.Order);
						if(order!=null)
							report.Scenario=order.Scenario;
						// your code here
						tradeRecords.Add(report);
					}
				}
			}
			//--- sort
			SortBindingList();
			//tradeRecords.s
		}

		private void SortBindingList()
		{
			backing.Sort((CTrade210 X,CTrade210 Y) => X.MainOrder.CompareTo(Y.MainOrder));
			// tell the bindinglist to raise a list change event so that 
			// bound controls reflect the new item order
			tradeRecords.ResetBindings();
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="engine"></param>
		/// <param name="e"></param>
		private void Engine_AfterReadRecord(EngineBase engine,FileHelpers.Events.AfterReadEventArgs<CTrade210> e)
		{
			switch(e.Record.Action)
			{
				case CustodyType.PA:
				case CustodyType.PK:
				case CustodyType.EM:
				case CustodyType.IP:
				//case CustodyType.UC:
				case CustodyType.TP:
				case CustodyType.SL:
					e.Record.Amount=e.Record.ContractSize*e.Record.Volume/100.0;
					break;
				default:
					e.SkipThisRecord=true;
					break;
			}
		}

		/// <summary>
		/// read fix messages
		/// </summary>
		private void ReadFixMessages()
		{
			string fileName = folderBox.Text;
			DirectoryInfo directory = new DirectoryInfo(fileName);
			//
			messages.Clear();
			dtMessages.Clear();
			//---
			foreach(var file in directory.GetFiles())
			{
				if(!file.Name.StartsWith("IDC.FIX.order"))
					continue;

				var engine = new FileHelperAsyncEngine<CFixMessage>();
				engine.ErrorMode=ErrorMode.IgnoreAndContinue;
				//engine.BeforeReadRecord += BeforeReadRecord;
				engine.AfterReadRecord+=FixMessageAfterReadRecord;

				// Read
				using(engine.BeginReadFile(file.FullName))
				{
					// The engine is IEnumerable
					foreach(CFixMessage log in engine)
					{
						// your code here
						//Debug.WriteLine((log.Context.Contains("to")?"=>":"<=")+" "+log.Message);

						dtMessages.Rows.Add(new object[] { log.Time,log.InOut,log.Message });
						//---------------test---------------// 
						messages.Add(log);
						//----------------------------------//
						//records.Add(order);
					}
				}
			}
			//--- endof loop
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="engine"></param>
		/// <param name="e"></param>
		private void FixMessageAfterReadRecord(EngineBase engine,FileHelpers.Events.AfterReadEventArgs<CFixMessage> e)
		{
			int index;
			//throw new NotImplementedException();
			if((index=e.Record.Context.IndexOf("8=FIX"))<0||e.Record.Context.Contains("|35=0"))
			{
				e.SkipThisRecord=true;
				return;
			}
			//---
			e.Record.Time=e.Record.Time.Replace("[","").Replace("]"," ");
			//---
			e.Record.Message=e.Record.Context.Substring(index);
			if(e.Record.Context.Contains("::to"))
				e.Record.InOut="=>";
			else if(e.Record.Context.Contains("::from"))
				e.Record.InOut="<=";
			else
				e.Record.Context=e.Record.Context.Substring(0,index);
		}

		private IMessageFactory _defaultMsgFactory = new DefaultMessageFactory();

		private void ordersGridView_SelectionChanged(object sender,EventArgs e)
		{
			Selected=null;

			if(!finished_reading)
				return;

			//dtMessages.DefaultView.RowFilter = "";

			int index = ordersGridView.SelectedRows[0].Index;
			if(index<0)
				return;

			//--- clear
			for(int row = 0;row<dictGridView.Rows.Count;row++)
			{
				//---
				dictGridView.Rows[row].Cells["FieldValue"].Value="";
				dictGridView.Rows[row].Cells["FieldExpected"].Value="";
			}

			Selected=tradeRecords[index];

			string order = ordersGridView["Order", index].Value?.ToString();

			string filter = string.Empty;
			string orderId = string.Empty;
			switch(Selected.Action)
			{
				case CustodyType.PA:
					orderId=ordersGridView["Comment",index].Value?.ToString().Replace("O:","");
					if(string.IsNullOrEmpty(orderId))
						return;
					//---
					filter=string.Format("[{0}] LIKE '%{1}%'",filterField,orderId);
					break;
				case CustodyType.EM:
					filter=string.Format("[{0}] LIKE '%order:{1}%'",filterField,order);
					break;
				case CustodyType.PK:
				case CustodyType.IP:
				case CustodyType.TP:
				case CustodyType.SL:
					filter=string.Format("[{0}] LIKE '%order:{1}%'",filterField,order);
					break;
				default:
					return;
			}

			#region find order in report

			//List<COrders> trades = new List<COrders>();
			//if (ulong.TryParse(order, out ulong order_int))
			//{
			//  trades = records.FindAll(o => o.Order.Equals(order_int));
			//}
			////---
			//if (trades.Count < 1) return;

			#endregion



			//orderId = ordersGridView["Comment", index].Value?.ToString().Replace("O:", "");
			//if (!string.IsNullOrEmpty(orderId))
			//{

			//}
			if(Selected.Action==CustodyType.PA)
			{
				try
				{

					var data = messages.FirstOrDefault(x => x.Message.Contains("37=" + orderId));
					if(data!=null)
					{
						data.Message=data.Message.Replace("|","\x01");
						QuickFix.Message message = new QuickFix.Message(data.Message, false);

						message.FromString(data.Message,false,null,null,_defaultMsgFactory);

						string clordId = GetString(message, 11);

						filter+=string.Format("OR [{0}] LIKE '%{1}%'",filterField,clordId);
					}
				}
				catch(Exception ex)
				{
					MessageBox.Show(ex.Message);
				}
			}
			else if(Selected.Action==CustodyType.EM)
			{
				filter+=string.Format("AND [{0}] LIKE '%idc:{1}%'",filterField,"A");
			}
			else
			{
				filter+=string.Format("AND [{0}] NOT LIKE '%idc:{1}%'",filterField,"A");
			}
			//--- filter
			dtMessages.DefaultView.RowFilter=filter;
		}

		private void Form1_Load(object sender,EventArgs e)
		{
			folderBox.Text=Properties.Settings.Default.folder;

			dtMessages.Columns.Add("Time",typeof(string));
			dtMessages.Columns.Add("InOut",typeof(string));
			dtMessages.Columns.Add("Message",typeof(string));
			//---
			messagesGridView.DataSource=dtMessages;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void messagesGridView_CellFormatting(object sender,DataGridViewCellFormattingEventArgs e)
		{
			//--- checks
			if(e.ColumnIndex<0||e.RowIndex<0)
				return;

			// Compare the column to the column you want to format
			if(this.messagesGridView.Columns[e.ColumnIndex].Name=="Message")
			{
				//get the item you are currently binding, using the index of the current row to access the datasource
				string item = messagesGridView.Rows[e.RowIndex].Cells[e.ColumnIndex].Value.ToString();
				string side = messagesGridView.Rows[e.RowIndex].Cells["InOut"].Value.ToString();
				//check the condition
				if(item.Contains("35=D"))
				{
					messagesGridView.Rows[e.RowIndex].DefaultCellStyle.BackColor=Color.LightSkyBlue;
					messagesGridView.Rows[e.RowIndex].DefaultCellStyle.ForeColor=Color.White;
				}
				else if(item.Contains("35=8")&&side.Equals("=>"))
				{
					messagesGridView.Rows[e.RowIndex].DefaultCellStyle.BackColor=Color.LimeGreen;
					messagesGridView.Rows[e.RowIndex].DefaultCellStyle.ForeColor=Color.White;
				}
				else if(item.Contains("35=Q"))
				{
					messagesGridView.Rows[e.RowIndex].DefaultCellStyle.BackColor=Color.PaleVioletRed;
					messagesGridView.Rows[e.RowIndex].DefaultCellStyle.ForeColor=Color.White;
				}
				else if(item.Contains("35=H"))
				{
					messagesGridView.Rows[e.RowIndex].DefaultCellStyle.BackColor=Color.LightYellow;
					messagesGridView.Rows[e.RowIndex].DefaultCellStyle.ForeColor=Color.Black;
				}
				else
				{
					messagesGridView.Rows[e.RowIndex].DefaultCellStyle.BackColor=messagesGridView.BackColor;
					messagesGridView.Rows[e.RowIndex].DefaultCellStyle.ForeColor=messagesGridView.DefaultCellStyle.ForeColor;
				}
			}
		}

		private void folderBox_TextChanged(object sender,EventArgs e)
		{
			Properties.Settings.Default.folder=folderBox.Text;

			Properties.Settings.Default.Save();
		}

		private readonly TextInfo textInfo = CultureInfo.CurrentCulture.TextInfo;
		//private readonly List<CKeyValuePair> keyvalue = new List<CKeyValuePair>();
		private void messagesGridView_CellClick(object sender,DataGridViewCellEventArgs e)
		{
			//--- cehcks
			if(e.RowIndex<0||e.ColumnIndex<0)
				return;
			//---
			try
			{
				var message = messagesGridView.Rows[e.RowIndex].Cells["Message"].Value.ToString().Replace("|", "\x01");
				QuickFix.Message fix_message = new QuickFix.Message(message,dataDictionary, false);

				var array=fix_message.ToList();
				array.AddRange(fix_message.Header.ToList());
				array.AddRange(fix_message.Trailer.ToList());
				//--- clear
				for(int row = 0;row<dictGridView.Rows.Count;row++)
				{
					dictGridView.Rows[row].Cells["FieldValue"].Value="";
					dictGridView.Rows[row].Cells["FieldExpected"].Value="";
				}
				//--- loop
				for(int row = 0;row<dictGridView.Rows.Count;row++)
				{
					int tag=int.Parse(dictGridView.Rows[row].Cells["FieldTag"].Value.ToString());
					//---
					var value = array.FirstOrDefault(x => x.Key == tag);
					if(value.Value==null)
						continue;

					var field=dataDictionary.FieldsByTag[tag];

					if(field.EnumDict.Count>0)
					{
						var data=field.EnumDict.FirstOrDefault(x=>x.Key==value.Value.ToString());
						dictGridView.Rows[row].Cells["FieldValue"].Value=textInfo.ToTitleCase(data.Value.Replace("_"," ").ToLower());
						;
					}
					else
						dictGridView.Rows[row].Cells["FieldValue"].Value=value.Value.ToString();
					//---
					if(Selected==null)
						return;

					switch(tag)
					{
						case 54:
							dictGridView.Rows[row].Cells["FieldExpected"].Value=Selected.Cmd;
							break;
						case 55:
							dictGridView.Rows[row].Cells["FieldExpected"].Value=Selected.Symbol;
							break;
						case 38:
							dictGridView.Rows[row].Cells["FieldExpected"].Value=Selected.MainVolume*Selected.ContractSize/100.0;
							break;
						case 14:
							dictGridView.Rows[row].Cells["FieldExpected"].Value=Selected.Amount;
							break;
						case 31:
							dictGridView.Rows[row].Cells["FieldExpected"].Value=(Selected.Action==CustodyType.PA) ? Selected.OpenPrice : Selected.ClosePrice;
							break;
						case 151:
							dictGridView.Rows[row].Cells["FieldExpected"].Value=(Selected.MainVolume-Selected.Volume)*Selected.ContractSize/100.0;
							break;
						default:
							break;
					}
				}
			}
			catch(Exception ex)
			{
				MessageBox.Show(ex.Message);
			}
		}
		/// <summary>
		/// 
		/// </summary>
		/// <param name="fix_message"></param>
		/// <param name="tag"></param>
		/// <returns></returns>
		string GetString(QuickFix.Message fix_message,int tag)
		{
			string str = "";
			try
			{
				str=fix_message.GetString(tag);
			}
			catch(Exception)
			{
			}
			//---
			return str;
		}
		/// <summary>
		/// 
		/// </summary>
		/// <param name="fix_message"></param>
		/// <param name="tag"></param>
		/// <returns></returns>
		string GetHeaderString(QuickFix.Message fix_message,int tag)
		{
			string str = "";
			try
			{
				str=fix_message.Header.GetString(tag);
			}
			catch(Exception)
			{
			}
			//---
			return str;
		}

		private BindingList<Instrument> instruments;
		private void btnInstruments_Click(object sender,EventArgs e)
		{
			instruments=new BindingList<Instrument>();
			//---
			var m_scenario = new Scenario(instruments);
			m_scenario.ShowDialog();
		}

		private readonly QuickFix.DataDictionary.DataDictionary dataDictionary=new QuickFix.DataDictionary.DataDictionary("FIX43.xml");

		private void messagesGridView_CellDoubleClick(object sender,DataGridViewCellEventArgs e)
		{
			//--- cehcks
			if(e.RowIndex<0||e.ColumnIndex<0)
				return;
			//---
			try
			{
				var message = messagesGridView.Rows[e.RowIndex].Cells["Message"].Value.ToString().Replace("|", "\x01");
				QuickFix.Message fix_message = new QuickFix.Message(message,dataDictionary, false);
				//---
				var array=fix_message.Header.ToList();
				array.AddRange(fix_message.ToList());
				array.AddRange(fix_message.Trailer.ToList());
				//----
				BindingList<CFixData> datum=new BindingList<CFixData>();

				foreach(var data in array)
				{
					var field=dataDictionary.FieldsByTag[data.Key];

					datum.Add(new CFixData()
					{
						Key=data.Key,
						Description=dataDictionary.FieldsByTag[data.Key].Name,
						Value=(field.EnumDict.Count>0) ? textInfo.ToTitleCase(field.EnumDict.FirstOrDefault(x => x.Key==data.Value.ToString()).Value.Replace("_"," ").ToLower()) : data.Value.ToString()
					});
				}

				var form=new FixMessage(datum);
				form.Show();
			}
			catch(Exception ex)
			{
				MessageBox.Show(ex.Message);
			}
		}
		//---
	}
}
