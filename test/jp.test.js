import {expect,test,beforeAll,afterAll} from "bun:test"
import { exec } from "node:child_process"
// import "bun:os"
import {platform} from "node:os"

beforeAll(async()=>{
    let make = "lin"
    if(platform()==="win32"){
        make = "win"
    }
    const proc =  Bun.spawn(["make",make])
    await proc.exited
})

async function runScript(file_path){
    // if(os.plat )
    // console.log(platform())
    let run_ex = "./jp"
    if(platform()==="win32"){
       run_ex = "./jp-win.exe"
    }
    const proc = Bun.spawn([run_ex,"-f",`${file_path}`,"--test"])
    const output = (await new Response(proc.stdout).text())

    await proc.exited

    return output
}

test("Check for Valid Empty JSON",async()=>{
    // const json_string = "{}"
    const output = await runScript("test/tests_file/test1.json")
    expect(output).toEqual("Succesfully Parsed\n")
})

test("Checking for single key value pair",async()=>{
    const output = await runScript("test/tests_file/test2.json")
    expect(output).toEqual("Succesfully Parsed\nkey:value\n")
})

test("Checking for more than one key value pair",async()=>{
    const output = await runScript("test/tests_file/test3.json")
    expect(output).toEqual("Succesfully Parsed\nkey1:value1\nkey2:value2\nkey$:value[];\n")
})

test("Checking for Object as values",async()=>{
    const output = await runScript("test/tests_file/test6.json")
    expect(output).toEqual("Succesfully Parsed\nkey1:<Object>\n")
})

test("Checking for Integer as values",async()=>{
    const output = await runScript("test/tests_file/test7.json")
    expect(output).toEqual("Succesfully Parsed\nkey:1\n")
})

test("Checking for Array as values",async()=>{
    const output = await runScript("test/tests_file/test8.json")
    expect(output).toEqual("Succesfully Parsed\nkey: <Array valuearray >\n");
})

test("Checking for Object Values in Array",async()=>{
    const output = await runScript("test/tests_file/test9.json")
    expect(output).toEqual("Succesfully Parsed\nkey: <Array <Object> >\n")
})

test("Checking for Array in Array Values",async()=>{
    const output = await runScript("test/tests_file/test10.json")
    expect(output).toEqual("Succesfully Parsed\nkey: <Array <Array 1> >\n")
})

// test("Checking for Values types",async()=>{
//     const output = await runScript("test/tests_file/test11.json")
//     expect(output).toEqual("Succesfully Parsed\n\e[1;32mSTRING\n\e[1;32mINTEGER\n\e[1;32mARRAY\n\e[1;32mOBJECT\n")
// })

test("Checking for Array as base value",async()=>{
    const output = await runScript("test/tests_file/test11.json")
    expect(output).toEqual("Succesfully Parsed\n<Array hi>\n")
})

test("Checking for Null value",async()=>{
    const output = await runScript("test/tests_file/test13.json")
    expect(output).toEqual("Succesfully Parsed\n<Array null>\n")
})

test("Checking for error string as base value",async()=>{
    const output = await runScript("test/tests_file/test12.json")
    expect(output).toEqual("Parser Error: Invalid Opening Tags\nOperation Aborted\n")
})

test("Checking Invalid Syntax check",async()=>{
    const output = await runScript("test/tests_file/test4.json")
    expect(output).toEqual("Parser Error : Invalid Syntax\nOperation Aborted\n")
})

test("Check for Lexical Error",async()=>{
    const output = await runScript("test/tests_file/test5.json")
    expect(output).toEqual("Lexical Error: $ not valid\n")
})

afterAll(()=>{
    let stmnt = "clean-lin"
    if(platform()==="win32"){
        stmnt = "clean-win"
    }
    const proc = Bun.spawn(['make',stmnt])
    proc.exited
})

