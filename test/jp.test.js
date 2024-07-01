import {expect,test,beforeAll,afterAll} from "bun:test"

beforeAll(async()=>{
    const proc =  Bun.spawn(["make"])
    await proc.exited
})

async function runScript(file_path){
    const proc = Bun.spawn(["./jp","-f",`${file_path}`,"--test"])
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

test("Checking Invalid Syntax check",async()=>{
    const output = await runScript("test/tests_file/test4.json")
    expect(output).toEqual("Parser Error : Invalid Syntax\nOperation Aborted\n")
})

test("Check for Lexical Error",async()=>{
    const output = await runScript("test/tests_file/test5.json")
    expect(output).toEqual("Lexical Error: $ not valid\n")
})

afterAll(()=>{
    const proc = Bun.spawn(['make','clean'])
    proc.exited
})

