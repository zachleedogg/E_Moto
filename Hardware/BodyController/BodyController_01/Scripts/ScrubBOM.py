import xlrd
import xlwt

wb = xlrd.open_workbook('E_Moto_BOM.xlsx')


# grab the active worksheet
ws=wb.sheet_by_index(0)


#parse excel sheet and return a list of dicts
#each row is a dict with keys being header values
#and values being the cell value    
def parse_rows(active_sheet):
    num_rows = active_sheet.nrows
    num_cols = active_sheet.ncols
    header = [active_sheet.cell_value(0, cell).lower() for cell in range(num_cols)]
    for row_idx in range(0, num_rows):
        row_cell = [active_sheet.cell_value(row_idx, col_idx) for col_idx in range(num_cols)]
        yield dict(zip(header, row_cell))

#takes your list of dicts and
#deletes row according to a key-value pair        
def delete_row_by_entry(data, key, value):
    new_data = []
    for row in data:
        if row.get(key) != value:
            new_data.append(row)
        else:
            print("deleteing " + str(row))
    return new_data


#####################################################
#Put rows in a list
##################################################### 
data_by_row = []

for row in parse_rows(ws):
    data_by_row.append(row)

row_count = ws.nrows
column_count = ws.ncols

######################################################
#look for no-pop parts (board features, no-pop, etc...)
######################################################

data_by_row = delete_row_by_entry(data_by_row,"populate","n")

########################################################
#Consolidate multiple Ref Designators
########################################################
unique_refs = []
unique_refs.append("MPN")
new_data = dict()

#create the dict by MPN
for row in data_by_row:
    if row.get("mpn") not in unique_refs:
        unique_refs.append(row.get("mpn"))
        item = {'qty' : 0, 'refs' : 'none', 'footprint' : 'none', 'datasheet' : 'none'}
        item['footprint'] =  row.get("footprint")
        item['datasheet'] =  row.get("datasheet")
        item['refs'] = []
        new_data[row.get("mpn")] = item

#add refDes and qty to each item
for row in data_by_row[1:]:
    refdes = row.get("refs")
    mpn = row.get("mpn")
    refList = new_data[mpn]["refs"]
    refList.append(refdes)
    new_data[mpn]["refs"] = refList
    qty = new_data[mpn]["qty"]
    qty = qty + 1
    new_data[mpn]["qty"] = qty

########################################################    
#Create new Excel file and BOM from Dictionary
########################################################

book = xlwt.Workbook()
sheet = book.add_sheet("BOM")
sheet.write(0,0,label = "MPN",)
sheet.write(0,1,label = "QTY",)
sheet.write(0,2,label = "RefDes",)
sheet.write(0,3,label = "Footprint",)
sheet.write(0,4,label = "Datasheet",)
i = 1
for items in new_data:
    print(items)
    sheet.write(i,0, label = items)
    sheet.write(i, 1, label = new_data[items]["qty"])
    myString = ",".join(new_data[items]["refs"])
    sheet.write(i, 2, label = myString)
    sheet.write(i, 3, label = new_data[items]["footprint"])
    sheet.write(i, 4, label = new_data[items]["datasheet"])
    i = i+1
    
book.save("BOM.xlsx")
    